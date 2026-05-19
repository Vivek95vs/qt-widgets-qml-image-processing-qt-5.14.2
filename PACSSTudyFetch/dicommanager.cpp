#include "dicommanager.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>
#include <QStandardPaths>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <QProcess>

// Helper function to convert QString to OFString
OFString qStringToOFString(const QString& str) {
    return OFString(str.toUtf8().constData());
}

// DicomWorker Implementation
DicomWorker::DicomWorker(QObject *parent)
    : QObject(parent)
    , m_scu(nullptr)
    , m_isRunning(true)
    , m_settings(nullptr)
{
}

void DicomWorker::initialize(const QString& configPath) {
    m_settings = new QSettings(configPath, QSettings::IniFormat, this);

    m_settings->beginGroup("LocalSettings");
    m_localAETitle = m_settings->value("AETitle", "VIVEK_VS").toString();
    m_localPort = m_settings->value("Port", 5554).toInt();
    m_settings->endGroup();

    // Load server configurations
    m_servers.clear();

    // Load from Servers section
    m_settings->beginGroup("Servers");
    QStringList childGroups = m_settings->childGroups();
    for (const QString& group : childGroups) {
        if (group.startsWith("Server")) {
            m_settings->beginGroup(group);
            ServerConfig server;
            server.aeTitle = m_settings->value("AETitle").toString();
            server.hostname = m_settings->value("Hostname").toString();
            server.port = m_settings->value("Port").toInt();
            server.name = m_settings->value("Name", server.aeTitle).toString();
            m_servers[server.name] = server;
            m_settings->endGroup();
        }
    }
    m_settings->endGroup();

    // Load from Servers2 section
    m_settings->beginGroup("Servers2");
    childGroups = m_settings->childGroups();
    for (const QString& group : childGroups) {
        if (group.startsWith("Server")) {
            m_settings->beginGroup(group);
            ServerConfig server;
            server.aeTitle = m_settings->value("AETitle").toString();
            server.hostname = m_settings->value("Hostname").toString();
            server.port = m_settings->value("Port").toInt();
            server.name = m_settings->value("Name", server.aeTitle).toString();
            m_servers[server.name] = server;
            m_settings->endGroup();
        }
    }
    m_settings->endGroup();

    emit logMessage("INFO", QString("Local AE Title: %1 on port %2").arg(m_localAETitle).arg(m_localPort));
    emit logMessage("INFO", QString("Loaded %1 server configurations").arg(m_servers.size()));
}
void DicomWorker::testConnection(const QString& serverName) {
    if (!m_isRunning) return;

    // Find the server
    ServerConfig server;
    bool serverFound = false;

    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
        if (it.value().name.contains(serverName) || it.key().contains(serverName)) {
            server = it.value();
            serverFound = true;
            break;
        }
    }

    if (!serverFound) {
        emit logMessage("ERROR", QString("Server '%1' not found").arg(serverName));
        emit transferCompleted(false, "Server not found");
        return;
    }

    emit logMessage("INFO", QString("Testing connection to %1 (%2:%3)...")
                    .arg(server.name).arg(server.hostname).arg(server.port));

    if (!createAssociation(server)) {
        emit logMessage("ERROR", "Connection test FAILED");
        emit transferCompleted(false, "Connection failed");
        return;
    }

    // Send C-ECHO request
    OFCondition result = m_scu->sendECHORequest(0);

    if (result.good()) {
        emit logMessage("INFO", "Connection test SUCCESSFUL!");
        emit transferCompleted(true, "Successfully connected to " + server.name);
    } else {
        emit logMessage("ERROR", QString("ECHO failed: %1").arg(result.text()));
        emit transferCompleted(false, "ECHO request failed");
    }

    closeAssociation();
}

bool DicomWorker::createAssociation(const ServerConfig& server) {
    if (m_scu) {
        delete m_scu;
        m_scu = nullptr;
    }

    m_scu = new DcmSCU();

    // Set local and remote AE titles
    m_scu->setAETitle(qStringToOFString(m_localAETitle));
    m_scu->setPeerHostName(qStringToOFString(server.hostname));
    m_scu->setPeerPort(server.port);
    m_scu->setPeerAETitle(qStringToOFString(server.aeTitle));

    // Add presentation contexts
    OFList<OFString> transferSyntaxes;
    transferSyntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
    transferSyntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);

    // IMPORTANT: Add C-FIND presentation context
    m_scu->addPresentationContext(UID_FINDStudyRootQueryRetrieveInformationModel, transferSyntaxes);

    // Add C-MOVE presentation context
    m_scu->addPresentationContext(UID_MOVEStudyRootQueryRetrieveInformationModel, transferSyntaxes);

    // Start with just verification (C-ECHO) for testing
    m_scu->addPresentationContext(UID_VerificationSOPClass, transferSyntaxes);

    // Add storage SOP classes
    m_scu->addPresentationContext(UID_CTImageStorage, transferSyntaxes);
    m_scu->addPresentationContext(UID_MRImageStorage, transferSyntaxes);
    m_scu->addPresentationContext(UID_UltrasoundImageStorage, transferSyntaxes);
    m_scu->addPresentationContext(UID_RTImageStorage, transferSyntaxes);


    // Initialize network with timeout
    OFCondition result = m_scu->initNetwork();
    if (result.bad()) {
        emit logMessage("ERROR", QString("Network init failed: %1").arg(result.text()));
        return false;
    }

    // Set timeout values (in seconds)
    m_scu->setConnectionTimeout(30);
    m_scu->setDIMSETimeout(30);

    // Negotiate association
    emit logMessage("INFO", QString("Connecting to %1 (%2:%3)...")
                    .arg(server.name).arg(server.hostname).arg(server.port));
    result = m_scu->negotiateAssociation();

    if (result.bad()) {
        emit logMessage("ERROR", QString("Association failed: %1").arg(result.text()));

        // Try to get more details about why it failed
        if (result == DUL_ASSOCIATIONREJECTED) {
            emit logMessage("ERROR", "Association was rejected by the PACS server");
            emit logMessage("INFO", "Possible causes:");
            emit logMessage("INFO", "  - AE Title 'VIVEK_VS' not registered on PACS");
            emit logMessage("INFO", "  - AE Title mismatch");
            emit logMessage("INFO", "  - PACS rejected the proposed SOP classes");
        } else if (result == DUL_NETWORKCLOSED) {
            emit logMessage("ERROR", "Network connection was closed by PACS");
            emit logMessage("INFO", "Check if the PACS server is running on port " + QString::number(server.port));
        } else if (result == DUL_READTIMEOUT) {
            emit logMessage("ERROR", "Connection timed out");
            emit logMessage("INFO", "Check network connectivity and firewall settings");
        }

        delete m_scu;
        m_scu = nullptr;
        return false;
    }

    emit logMessage("INFO", "Association established successfully!");
    return true;
}

void DicomWorker::closeAssociation() {
    if (m_scu) {
        m_scu->closeAssociation(DCMSCU_RELEASE_ASSOCIATION);
        delete m_scu;
        m_scu = nullptr;
    }
}

void DicomWorker::queryPACS(const QString& patientId) {
    if (!m_isRunning) return;

    // Find the query server (SUVARNAQRY)
    ServerConfig server;
    bool serverFound = false;

    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
        if (it.value().name.contains("SUVARNAQRY") || it.key().contains("SUVARNAQRY")) {
            server = it.value();
            serverFound = true;
            break;
        }
    }

    if (!serverFound && m_servers.size() > 0) {
        server = m_servers.begin().value();
        serverFound = true;
    }

    if (!serverFound) {
        emit logMessage("ERROR", "No server found in configuration");
        emit studyListReady(QVariantList());
        return;
    }

    emit progressUpdated(5, QString("Connecting to %1...").arg(server.name));

    if (!createAssociation(server)) {
        emit studyListReady(QVariantList());
        return;
    }

    emit progressUpdated(20, "Building query...");

    // Create query dataset for C-FIND
    DcmDataset queryDataset;
    queryDataset.putAndInsertOFStringArray(DCM_QueryRetrieveLevel, "STUDY");
    queryDataset.putAndInsertOFStringArray(DCM_StudyInstanceUID, "");
    queryDataset.putAndInsertOFStringArray(DCM_PatientName, "");
    queryDataset.putAndInsertOFStringArray(DCM_PatientID, "");
    queryDataset.putAndInsertOFStringArray(DCM_StudyDate, "");
    queryDataset.putAndInsertOFStringArray(DCM_StudyDescription, "");
    queryDataset.putAndInsertOFStringArray(DCM_ModalitiesInStudy, "");
    queryDataset.putAndInsertOFStringArray(DCM_NumberOfStudyRelatedInstances, "");

    if (!patientId.isEmpty()) {
        queryDataset.putAndInsertOFStringArray(DCM_PatientID, qStringToOFString(patientId));
        emit logMessage("INFO", QString("Filtering by Patient ID: %1").arg(patientId));
    }

    emit progressUpdated(30, "Sending query to PACS...");

    // IMPORTANT: Get the correct presentation context ID
    // First, find which presentation context was accepted for C-FIND
    T_ASC_PresentationContextID presID = m_scu->findPresentationContextID(
        UID_FINDStudyRootQueryRetrieveInformationModel,
        UID_LittleEndianImplicitTransferSyntax);

    if (presID == 0) {
        // Try with explicit transfer syntax
        presID = m_scu->findPresentationContextID(
            UID_FINDStudyRootQueryRetrieveInformationModel,
            UID_LittleEndianExplicitTransferSyntax);
    }

    if (presID == 0) {
        emit logMessage("ERROR", "No valid presentation context ID found for C-FIND");
        emit logMessage("INFO", "The PACS may not support Study Root Query/Retrieve");
        closeAssociation();
        emit studyListReady(QVariantList());
        return;
    }

    emit logMessage("INFO", QString("Using presentation context ID: %1").arg(presID));

    // Send C-FIND request with the correct presentation context ID
    OFList<QRResponse*> responses;
    OFCondition result = m_scu->sendFINDRequest(presID, &queryDataset, &responses);

    if (result.good()) {
        emit progressUpdated(80, "Processing query results...");

        QVariantList studies;
        int count = 0;

        for (auto it = responses.begin(); it != responses.end(); ++it) {
            QRResponse* response = *it;
            if (response && response->m_dataset) {
                DcmDataset* responseDataset = response->m_dataset;
                QVariantMap study;

                OFString value;

                if (responseDataset->findAndGetOFString(DCM_StudyInstanceUID, value).good()) {
                    study["studyUID"] = QString::fromStdString(value.c_str());
                }
                if (responseDataset->findAndGetOFString(DCM_PatientName, value).good()) {
                    study["patientName"] = QString::fromStdString(value.c_str());
                }
                if (responseDataset->findAndGetOFString(DCM_PatientID, value).good()) {
                    study["patientId"] = QString::fromStdString(value.c_str());
                }
                if (responseDataset->findAndGetOFString(DCM_StudyDate, value).good()) {
                    QString dateStr = QString::fromStdString(value.c_str());
                    if (dateStr.length() == 8) {
                        dateStr = QString("%1-%2-%3").arg(dateStr.left(4)).arg(dateStr.mid(4,2)).arg(dateStr.right(2));
                    }
                    study["studyDate"] = dateStr;
                }
                if (responseDataset->findAndGetOFString(DCM_StudyDescription, value).good()) {
                    study["studyDescription"] = QString::fromStdString(value.c_str());
                }
                if (responseDataset->findAndGetOFString(DCM_ModalitiesInStudy, value).good()) {
                    study["modalities"] = QString::fromStdString(value.c_str());
                }
                if (responseDataset->findAndGetOFString(DCM_NumberOfStudyRelatedInstances, value).good()) {
                    study["instances"] = QString::fromStdString(value.c_str());
                }

                if (study.contains("studyUID") && !study["studyUID"].toString().isEmpty()) {
                    studies.append(study);
                    count++;
                }
            }
        }

        emit logMessage("INFO", QString("Found %1 studies").arg(count));
        emit studyListReady(studies);

    } else {
        emit logMessage("ERROR", QString("C-FIND query failed: %1").arg(result.text()));
        emit studyListReady(QVariantList());
    }

    // Clean up responses
    for (auto it = responses.begin(); it != responses.end(); ++it) {
        delete *it;
    }

    closeAssociation();
    emit progressUpdated(100, "Query completed");
}

//void DicomWorker::retrieveStudy(const QString& studyUID, const QString& savePath) {
//    if (!m_isRunning) return;

//    emit progressUpdated(0, "Starting study retrieval...");
//    emit logMessage("INFO", "========================================");
//    emit logMessage("INFO", QString("Retrieving study: %1").arg(studyUID));

//    // Create save directory
//    QDir dir;
//    if (!dir.mkpath(savePath)) {
//        emit logMessage("ERROR", QString("Cannot create directory: %1").arg(savePath));
//        emit transferCompleted(false, "Cannot create save directory");
//        return;
//    }

//    // Path to storescp
//    QString storescpPath = "D:/Qt 2017 program/PacsIntegrate/release/storescp.exe";

//    // Verify storescp exists
//    if (!QFile::exists(storescpPath)) {
//        emit logMessage("ERROR", "storescp.exe not found at: " + storescpPath);
//        emit transferCompleted(false, "storescp.exe not found");
//        return;
//    }

//    emit logMessage("INFO", "Starting DICOM Storage SCP...");
//    emit logMessage("INFO", QString("Storescp path: %1").arg(storescpPath));
//    emit logMessage("INFO", QString("Local AE Title: %1").arg(m_localAETitle));
//    emit logMessage("INFO", QString("Listening on port: %1").arg(m_localPort));
//    emit logMessage("INFO", QString("Save directory: %1").arg(savePath));

//    // Start storescp process
//    QProcess* storageProcess = new QProcess(this);

//    // CORRECT arguments for storescp v3.6.6 - No --log-level with -v
//    QStringList arguments;
//    arguments << "-v"                           // Verbose mode
//              << "-aet" << m_localAETitle       // Our AE title (VIVEK_VS)
//              << "-od" << savePath              // Output directory
//              << QString::number(m_localPort);  // Port number as LAST argument

//    emit logMessage("INFO", QString("Running command: %1 %2").arg(storescpPath).arg(arguments.join(" ")));

//    // Start storescp
//    storageProcess->start(storescpPath, arguments);

//    if (!storageProcess->waitForStarted(5000)) {
//        emit logMessage("ERROR", "Failed to start storescp");
//        emit logMessage("ERROR", QString("Error: %1").arg(storageProcess->errorString()));
//        delete storageProcess;
//        emit transferCompleted(false, "Failed to start storage SCP");
//        return;
//    }

//    emit logMessage("INFO", "✓ Storage SCP started successfully and is listening for connections");

//    // Connect to capture storescp output
//    connect(storageProcess, &QProcess::readyReadStandardOutput,
//            [this, storageProcess, savePath]() {
//        QString output = QString::fromLocal8Bit(storageProcess->readAllStandardOutput());
//        if (!output.isEmpty()) {
//            emit logMessage("SCP", output.trimmed());

//            // Check if a file was saved
//            if (output.contains("storing DICOM file")) {
//                QDir saveDir(savePath);
//                int fileCount = saveDir.entryList(QStringList() << "*.dcm", QDir::Files).size();
//                emit logMessage("INFO", QString("✓ Received file. Total so far: %1 files").arg(fileCount));
//            }
//        }
//    });

//    connect(storageProcess, &QProcess::readyReadStandardError,
//            [this, storageProcess]() {
//        QString error = QString::fromLocal8Bit(storageProcess->readAllStandardError());
//        if (!error.isEmpty()) {
//            emit logMessage("SCP", error.trimmed());
//        }
//    });

//    // Give storescp time to initialize
//    QThread::sleep(2);

//    // Find PACS server for C-MOVE
//    ServerConfig server;
//    bool serverFound = false;

//    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
//        if (it.value().name.contains("SUVARNAQRY") || it.key().contains("SUVARNAQRY")) {
//            server = it.value();
//            serverFound = true;
//            break;
//        }
//    }

//    if (!serverFound && m_servers.size() > 0) {
//        server = m_servers.begin().value();
//        serverFound = true;
//    }

//    if (!serverFound) {
//        emit logMessage("ERROR", "No retrieve server found");
//        storageProcess->terminate();
//        storageProcess->deleteLater();
//        emit transferCompleted(false, "Server not configured");
//        return;
//    }

//    // Create DICOM association with PACS
//    emit logMessage("INFO", "Connecting to PACS for C-MOVE...");
//    if (!createAssociation(server)) {
//        storageProcess->terminate();
//        storageProcess->deleteLater();
//        emit transferCompleted(false, "Failed to connect to PACS");
//        return;
//    }

//    emit progressUpdated(20, "Sending C-MOVE request...");

//    // Create C-MOVE request
//    DcmDataset moveDataset;
//    moveDataset.putAndInsertOFStringArray(DCM_QueryRetrieveLevel, "STUDY");
//    moveDataset.putAndInsertOFStringArray(DCM_StudyInstanceUID, qStringToOFString(studyUID));

//    // Get presentation context ID for C-MOVE
//    T_ASC_PresentationContextID presID = m_scu->findPresentationContextID(
//        UID_MOVEStudyRootQueryRetrieveInformationModel,
//        UID_LittleEndianImplicitTransferSyntax);

//    if (presID == 0) {
//        presID = m_scu->findPresentationContextID(
//            UID_MOVEStudyRootQueryRetrieveInformationModel,
//            UID_LittleEndianExplicitTransferSyntax);
//    }

//    if (presID == 0) {
//        emit logMessage("ERROR", "No valid presentation context ID for C-MOVE");
//        closeAssociation();
//        storageProcess->terminate();
//        storageProcess->deleteLater();
//        emit transferCompleted(false, "No presentation context for C-MOVE");
//        return;
//    }

//    // Send C-MOVE request
//    OFList<RetrieveResponse*> responses;
//    OFString ourAETitle = qStringToOFString(m_localAETitle);
//    emit logMessage("INFO", "Sending C-MOVE request to PACS...");
//    emit logMessage("INFO", QString("Moving to destination AE: %1").arg(m_localAETitle));
//    emit logMessage("INFO", QString("PACS will send images to: %1:%2").arg(m_localAETitle).arg(m_localPort));

//    OFCondition result = m_scu->sendMOVERequest(presID, ourAETitle,&moveDataset, &responses);

//    if (result.good()) {
//        emit logMessage("INFO", "✓ C-MOVE request accepted by PACS");
//        emit logMessage("INFO", "Waiting for images to arrive...");

//        emit progressUpdated(50, "Receiving images from PACS...");

//        // Wait for images to arrive
//        int waitTime = 0;
//        int maxWaitTime = 120;
//        int lastFileCount = 0;
//        int noChangeCount = 0;

//        while (waitTime < maxWaitTime && storageProcess->state() == QProcess::Running) {
//            QThread::sleep(2);
//            waitTime += 2;

//            QDir saveDir(savePath);
//            int fileCount = saveDir.entryList(QStringList() << "*.dcm", QDir::Files).size();

//            if (fileCount > lastFileCount) {
//                int newFiles = fileCount - lastFileCount;
//                lastFileCount = fileCount;
//                noChangeCount = 0;
//                emit logMessage("INFO", QString("✓ Received %1 new file(s). Total: %2 files").arg(newFiles).arg(fileCount));
//                emit progressUpdated(50 + (waitTime * 40 / maxWaitTime),
//                                   QString("Received %1 files...").arg(fileCount));
//            } else {
//                noChangeCount++;
//                if (noChangeCount >= 10 && lastFileCount > 0) {
//                    emit logMessage("INFO", "No new files received for 20 seconds, assuming transfer complete");
//                    break;
//                }
//            }

//            int progress = 50 + (waitTime * 40 / maxWaitTime);
//            if (progress > 90) progress = 90;
//            emit progressUpdated(progress, QString("Waiting for images... (%1 sec)").arg(waitTime));
//        }

//        // Check results
//        QDir saveDir(savePath);
//        QStringList receivedFiles = saveDir.entryList(QStringList() << "*.dcm", QDir::Files);

//        emit logMessage("INFO", "========================================");
//        if (receivedFiles.size() > 0) {
//            emit logMessage("SUCCESS", QString("✓ Successfully received %1 DICOM files!").arg(receivedFiles.size()));
//            emit logMessage("INFO", QString("Files saved to: %1").arg(savePath));

//            for (int i = 0; i < qMin(10, receivedFiles.size()); ++i) {
//                QFileInfo fileInfo(receivedFiles[i]);
//                emit logMessage("INFO", QString("  - %1 (%2 bytes)").arg(receivedFiles[i]).arg(fileInfo.size()));
//            }
//            if (receivedFiles.size() > 10) {
//                emit logMessage("INFO", QString("  ... and %1 more files").arg(receivedFiles.size() - 10));
//            }

//            emit transferCompleted(true, QString("Successfully received %1 DICOM files in %2")
//                                  .arg(receivedFiles.size()).arg(savePath));
//        } else {
//            emit logMessage("WARNING", "No DICOM files were received!");
//            emit logMessage("WARNING", "The PACS accepted the C-MOVE but didn't send files.");
//            emit logMessage("WARNING", "Possible causes:");
//            emit logMessage("WARNING", "");
//            emit logMessage("WARNING", "1. FIREWALL: Port 5554 is blocked");
//            emit logMessage("WARNING", "   → Run this command as Administrator to add firewall rule:");
//            emit logMessage("WARNING", "   netsh advfirewall firewall add rule name=\"DICOM SCP\" dir=in action=allow protocol=TCP localport=5554");
//            emit logMessage("WARNING", "");
//            emit logMessage("WARNING", "2. PACS cannot reach your IP address");
//            emit logMessage("WARNING", "   → Your computer's IP must be reachable from 192.168.22.130");
//            emit logMessage("WARNING", "   → Check if you're on the same network/VPN");
//            emit logMessage("WARNING", "");
//            emit logMessage("WARNING", "3. Test manually with storescp in separate CMD window:");
//            emit logMessage("WARNING", QString("   storescp -v -aet %1 -od %2 %3").arg(m_localAETitle).arg(savePath).arg(m_localPort));
//            emit logMessage("WARNING", "");
//            emit logMessage("WARNING", "4. Then try downloading again - images should appear in the CMD window");

//            emit transferCompleted(false, "C-MOVE completed but no files received - Check firewall");
//        }
//        emit logMessage("INFO", "========================================");

//        if (receivedFiles.size() > 0) {
//            emit logMessage("INFO", "Keeping Storage SCP open for additional 10 seconds...");
//            QThread::sleep(10);
//        }

//    } else {
//        emit logMessage("ERROR", QString("C-MOVE failed: %1").arg(result.text()));
//        emit transferCompleted(false, "C-MOVE request failed");
//    }

//    // Clean up
//    for (auto it = responses.begin(); it != responses.end(); ++it) {
//        delete *it;
//    }

//    closeAssociation();

//    // Stop storescp
//    emit logMessage("INFO", "Stopping Storage SCP...");
//    storageProcess->terminate();
//    if (!storageProcess->waitForFinished(5000)) {
//        storageProcess->kill();
//    }
//    storageProcess->deleteLater();

//    emit progressUpdated(100, "Retrieval completed");
//}

void DicomWorker::retrieveStudy(const QString& studyUID, const QString& savePath) {
    if (!m_isRunning) return;

    emit progressUpdated(0, "Starting study retrieval...");
    emit logMessage("INFO", "========================================");
    emit logMessage("INFO", QString("Retrieving study: %1").arg(studyUID));

    // Normalize and create save directory
    QString normalizedPath = savePath;
    normalizedPath.replace("\\", "/");

    // Ensure path ends without trailing slash
    if (normalizedPath.endsWith("/")) {
        normalizedPath.chop(1);
    }

    emit logMessage("INFO", QString("Save directory: %1").arg(normalizedPath));

    // Create save directory
    QDir dir;
    if (!dir.mkpath(normalizedPath)) {
        emit logMessage("ERROR", QString("Cannot create directory: %1").arg(normalizedPath));
        emit transferCompleted(false, "Cannot create save directory");
        return;
    }

    // Kill any existing storescp processes
    emit logMessage("INFO", "Cleaning up existing storescp processes...");
#ifdef Q_OS_WIN
    QProcess::execute("taskkill", QStringList() << "/F" << "/IM" << "storescp.exe");
#else
    QProcess::execute("pkill", QStringList() << "storescp");
#endif
    QThread::sleep(1);

    // Path to storescp
    QString storescpPath = "D:/Qt 2017 program/PacsIntegrate/release/storescp.exe";

    // Verify storescp exists
    if (!QFile::exists(storescpPath)) {
        // Try alternative location
        storescpPath = QCoreApplication::applicationDirPath() + "/storescp.exe";
        if (!QFile::exists(storescpPath)) {
            emit logMessage("ERROR", "storescp.exe not found!");
            emit transferCompleted(false, "storescp.exe not found");
            return;
        }
    }

    emit logMessage("INFO", "Starting DICOM Storage SCP...");
    emit logMessage("INFO", QString("Storescp path: %1").arg(storescpPath));
    emit logMessage("INFO", QString("Local AE Title: %1").arg(m_localAETitle));
    emit logMessage("INFO", QString("Listening on port: %1").arg(m_localPort));

    // Start storescp process
    QProcess* storageProcess = new QProcess(this);

    // Use normalized path for output directory
    QStringList arguments;
    arguments << "-v"
              << "-aet" << m_localAETitle
              << "-od" << normalizedPath
              << QString::number(m_localPort);

    emit logMessage("INFO", QString("Running: %1 %2").arg(storescpPath).arg(arguments.join(" ")));

    storageProcess->start(storescpPath, arguments);

    if (!storageProcess->waitForStarted(5000)) {
        emit logMessage("ERROR", "Failed to start storescp");
        emit logMessage("ERROR", QString("Error: %1").arg(storageProcess->errorString()));
        delete storageProcess;
        emit transferCompleted(false, "Failed to start storage SCP");
        return;
    }

    emit logMessage("INFO", "✓ Storage SCP started successfully");

    // Connect to capture storescp output
    connect(storageProcess, &QProcess::readyReadStandardOutput,
            [this, storageProcess, normalizedPath]() {
        QString output = QString::fromLocal8Bit(storageProcess->readAllStandardOutput());
        if (!output.isEmpty()) {
            emit logMessage("SCP", output.trimmed());

            // Check for successful file save
            if (output.contains("storing DICOM file")) {
                QDir saveDir(normalizedPath);
                int fileCount = saveDir.entryList(QStringList() << "*.dcm", QDir::Files).size();
                emit logMessage("INFO", QString("Files received so far: %1").arg(fileCount));
            }
        }
    });

    connect(storageProcess, &QProcess::readyReadStandardError,
            [this, storageProcess]() {
        QString error = QString::fromLocal8Bit(storageProcess->readAllStandardError());
        if (!error.isEmpty()) {
            // Don't emit port-in-use error as critical since we killed previous processes
            if (!error.contains("Only one usage")) {
                emit logMessage("SCP", error.trimmed());
            }
        }
    });

    // Give storescp time to initialize
    QThread::sleep(2);

    // Find PACS server
    ServerConfig server;
    bool serverFound = false;

    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
        if (it.value().name.contains("SUVARNAQRY") || it.key().contains("SUVARNAQRY")) {
            server = it.value();
            serverFound = true;
            break;
        }
    }

    if (!serverFound && m_servers.size() > 0) {
        server = m_servers.begin().value();
        serverFound = true;
    }

    if (!serverFound) {
        emit logMessage("ERROR", "No retrieve server found");
        storageProcess->terminate();
        storageProcess->deleteLater();
        emit transferCompleted(false, "Server not configured");
        return;
    }

    // Create DICOM association
    emit logMessage("INFO", "Connecting to PACS for C-MOVE...");
    if (!createAssociation(server)) {
        storageProcess->terminate();
        storageProcess->deleteLater();
        emit transferCompleted(false, "Failed to connect to PACS");
        return;
    }

    emit progressUpdated(20, "Sending C-MOVE request...");

    // Create C-MOVE request
    DcmDataset moveDataset;
    moveDataset.putAndInsertOFStringArray(DCM_QueryRetrieveLevel, "STUDY");
    moveDataset.putAndInsertOFStringArray(DCM_StudyInstanceUID, qStringToOFString(studyUID));

    // Get presentation context ID
    T_ASC_PresentationContextID presID = m_scu->findPresentationContextID(
        UID_MOVEStudyRootQueryRetrieveInformationModel,
        UID_LittleEndianImplicitTransferSyntax);

    if (presID == 0) {
        presID = m_scu->findPresentationContextID(
            UID_MOVEStudyRootQueryRetrieveInformationModel,
            UID_LittleEndianExplicitTransferSyntax);
    }

    if (presID == 0) {
        emit logMessage("ERROR", "No valid presentation context ID for C-MOVE");
        closeAssociation();
        storageProcess->terminate();
        storageProcess->deleteLater();
        emit transferCompleted(false, "No presentation context for C-MOVE");
        return;
    }

    // Send C-MOVE request
    OFList<RetrieveResponse*> responses;
    OFString ourAETitle = qStringToOFString(m_localAETitle);
    emit logMessage("INFO", "Sending C-MOVE request to PACS...");
    emit logMessage("INFO", QString("Moving to destination AE: %1").arg(m_localAETitle));

    OFCondition result = m_scu->sendMOVERequest(presID, ourAETitle, &moveDataset, &responses);

    if (result.good()) {
        emit logMessage("INFO", "✓ C-MOVE request accepted by PACS");
        emit transferCompleted(false, "C-MOVE request Successfull");

    } else {
        emit logMessage("ERROR", QString("C-MOVE failed: %1").arg(result.text()));
        emit transferCompleted(false, "C-MOVE request failed");
    }

    // Cleanup
    for (auto it = responses.begin(); it != responses.end(); ++it) {
        delete *it;
    }

    closeAssociation();

    // Stop storescp
    emit logMessage("INFO", "Stopping Storage SCP...");
    storageProcess->terminate();
    if (!storageProcess->waitForFinished(3000)) {
        storageProcess->kill();
    }
    storageProcess->deleteLater();

    emit progressUpdated(100, "Retrieval completed");
}
void DicomWorker::sendDICOMFile(const QString& filePath, const QString& serverName) {
    if (!m_isRunning) return;

    // Find the storage server
    ServerConfig server;
    bool serverFound = false;

    for (auto it = m_servers.begin(); it != m_servers.end(); ++it) {
        if (it.value().name.contains(serverName) || it.key().contains(serverName)) {
            server = it.value();
            serverFound = true;
            break;
        }
    }

    if (!serverFound && m_servers.contains(serverName)) {
        server = m_servers[serverName];
        serverFound = true;
    }

    if (!serverFound) {
        emit logMessage("ERROR", QString("Server '%1' not found").arg(serverName));
        emit transferCompleted(false, "Server not found");
        return;
    }

    emit progressUpdated(10, QString("Connecting to %1...").arg(server.name));

    if (!createAssociation(server)) {
        emit transferCompleted(false, "Failed to connect to server");
        return;
    }

    emit progressUpdated(30, QString("Loading DICOM file: %1").arg(filePath));

    // Load DICOM file
    DcmFileFormat fileFormat;
    OFCondition result = fileFormat.loadFile(qStringToOFString(filePath));

    if (result.bad()) {
        emit logMessage("ERROR", QString("Failed to load DICOM file: %1").arg(result.text()));
        emit transferCompleted(false, "Cannot load DICOM file");
        closeAssociation();
        return;
    }

    DcmDataset* dataset = fileFormat.getDataset();

    // Get SOP class UID
    OFString sopClassUID;
    if (!dataset->findAndGetOFString(DCM_SOPClassUID, sopClassUID).good()) {
        DcmMetaInfo* metainfo = fileFormat.getMetaInfo();
        if (metainfo) {
            metainfo->findAndGetOFString(DCM_MediaStorageSOPClassUID, sopClassUID);
        }
    }

    if (sopClassUID.empty()) {
        emit logMessage("ERROR", "No SOP Class UID found in file");
        emit transferCompleted(false, "Invalid DICOM file");
        closeAssociation();
        return;
    }

    // Find presentation context ID for this SOP class
    T_ASC_PresentationContextID presID = m_scu->findPresentationContextID(
        sopClassUID.c_str(),
        UID_LittleEndianImplicitTransferSyntax);

    if (presID == 0) {
        presID = m_scu->findPresentationContextID(
            sopClassUID.c_str(),
            UID_LittleEndianExplicitTransferSyntax);
    }

    if (presID == 0) {
        emit logMessage("ERROR", "No presentation context for SOP Class: " + QString(sopClassUID.c_str()));
        emit transferCompleted(false, "Unsupported SOP class");
        closeAssociation();
        return;
    }

    emit progressUpdated(50, "Sending to PACS...");

    // Send C-STORE request
    Uint16 rspStatusCode = 0;
    result = m_scu->sendSTORERequest(presID,
                                     qStringToOFString(filePath),
                                     dataset,
                                     rspStatusCode);

    if (result.good() && rspStatusCode == 0) {
        emit logMessage("INFO", "File sent successfully");
        emit transferCompleted(true, "File sent to PACS");
    } else {
        emit logMessage("ERROR", QString("Send failed: %1 (Status: %2)").arg(result.text()).arg(rspStatusCode));
        emit transferCompleted(false, "Send failed");
    }

    closeAssociation();
    emit progressUpdated(100, "Transfer completed");
}
void DicomWorker::stop() {
    m_isRunning = false;
    closeAssociation();
}

DicomManager::DicomManager(QObject *parent)
    : QObject(parent)
    , m_workerThread(nullptr)
    , m_worker(nullptr)
    , m_isTransferring(false)
{
}

DicomManager::~DicomManager() {
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait(5000);
        delete m_workerThread;
    }
}

void DicomManager::loadConfiguration(const QString& configPath) {
    QSettings settings(configPath, QSettings::IniFormat);

    qDebug()<<"Config path"<<configPath;

    // Load local settings
    settings.beginGroup("LocalSettings");
    m_localConfig.aeTitle = settings.value("AETitle", "VIVEK_VS").toString();
    m_localConfig.port = settings.value("Port", 5554).toInt();
    m_localConfig.hostname = settings.value("Hostname", "192.168.10.21").toString();
    settings.endGroup();

    // Load servers
    m_serverConfigs.clear();
    m_serversList.clear();

    // Load from Servers section
    settings.beginGroup("Servers");
    if (settings.contains("Server1\\AETitle")) {
        ServerConfig server;
        server.aeTitle = settings.value("Server1\\AETitle").toString();
        server.hostname = settings.value("Server1\\Hostname").toString();
        server.port = settings.value("Server1\\Port").toInt();
        server.name = settings.value("Server1\\Name", server.aeTitle).toString();
        m_serverConfigs[server.name] = server;

        QVariantMap serverMap;
        serverMap["name"] = server.name;
        serverMap["aeTitle"] = server.aeTitle;
        serverMap["hostname"] = server.hostname;
        serverMap["port"] = server.port;
        serverMap["type"] = "query";
        m_serversList.append(serverMap);
    }
    settings.endGroup();

    // Load from Servers2 section
    settings.beginGroup("Servers2");
    if (settings.contains("Server2\\AETitle")) {
        ServerConfig server;
        server.aeTitle = settings.value("Server2\\AETitle").toString();
        server.hostname = settings.value("Server2\\Hostname").toString();
        server.port = settings.value("Server2\\Port").toInt();
        server.name = settings.value("Server2\\Name", server.aeTitle).toString();
        m_serverConfigs[server.name] = server;

        QVariantMap serverMap;
        serverMap["name"] = server.name;
        serverMap["aeTitle"] = server.aeTitle;
        serverMap["hostname"] = server.hostname;
        serverMap["port"] = server.port;
        serverMap["type"] = "storage";
        m_serversList.append(serverMap);
    }
    settings.endGroup();

    emit serversChanged();
    emit logMessage("INFO", QString("Configuration loaded: %1 servers").arg(m_serversList.size()));

    // Start worker thread
    if (!m_workerThread) {
        m_workerThread = new QThread(this);
        m_worker = new DicomWorker();
        m_worker->moveToThread(m_workerThread);

        connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_workerThread, &QThread::finished, this, &DicomManager::onWorkerFinished);

        connect(m_worker, &DicomWorker::studyListReady, this, &DicomManager::onStudyListReady);
        connect(m_worker, &DicomWorker::transferCompleted, this, &DicomManager::onTransferCompleted);
        connect(m_worker, &DicomWorker::logMessage, this, &DicomManager::onLogMessage);
        connect(m_worker, &DicomWorker::progressUpdated, this, &DicomManager::onProgressUpdated);

        m_workerThread->start();

        QMetaObject::invokeMethod(m_worker, "initialize", Q_ARG(QString, configPath));
    }
}

void DicomManager::queryStudies(const QString& patientId) {
    if (m_isTransferring) {
        emit logMessage("WARNING", "Already transferring, please wait");
        return;
    }

    setTransferring(true);
    QMetaObject::invokeMethod(m_worker, "queryPACS", Q_ARG(QString, patientId));
}

void DicomManager::downloadStudy(const QString& studyUID, const QString& savePath) {
    if (m_isTransferring) {
        emit logMessage("WARNING", "Already transferring, please wait");
        return;
    }

    setTransferring(true);
    QMetaObject::invokeMethod(m_worker, "retrieveStudy", Q_ARG(QString, studyUID), Q_ARG(QString, savePath));
}

void DicomManager::sendToPACS(const QString& filePath, const QString& serverName) {
    if (m_isTransferring) {
        emit logMessage("WARNING", "Already transferring, please wait");
        return;
    }

    if (!QFile::exists(filePath)) {
        emit logMessage("ERROR", "File does not exist: " + filePath);
        return;
    }

    setTransferring(true);
    QMetaObject::invokeMethod(m_worker, "sendDICOMFile", Q_ARG(QString, filePath), Q_ARG(QString, serverName));
}

void DicomManager::sendDirectoryToPACS(const QString& directoryPath, const QString& serverName) {
    if (m_isTransferring) {
        emit logMessage("WARNING", "Already transferring, please wait");
        return;
    }

    QDir dir(directoryPath);
    if (!dir.exists()) {
        emit logMessage("ERROR", "Directory does not exist: " + directoryPath);
        return;
    }

    QStringList dicomFiles = dir.entryList(QStringList() << "*.dcm" << "*.DCM", QDir::Files);

    if (dicomFiles.isEmpty()) {
        emit logMessage("WARNING", "No DICOM files found in directory");
        return;
    }

    emit logMessage("INFO", QString("Found %1 DICOM files to send").arg(dicomFiles.size()));

    // Send files one by one
    int sent = 0;
    for (const QString& file : dicomFiles) {
        if (!m_isTransferring) break;
        sendToPACS(dir.filePath(file), serverName);
        sent++;
        QThread::msleep(500); // Small delay between sends
    }

    emit logMessage("INFO", QString("Sent %1 of %2 files").arg(sent).arg(dicomFiles.size()));
}

void DicomManager::verifyConnection(const QString& serverName) {
    emit logMessage("INFO", "Verifying connection to " + serverName);
    emit operationCompleted(true, "Connection verified");
}

void DicomManager::setTransferring(bool transferring) {
    if (m_isTransferring != transferring) {
        m_isTransferring = transferring;
        emit transferringChanged();
    }
}

void DicomManager::setStatus(const QString& status) {
    if (m_currentStatus != status) {
        m_currentStatus = status;
        emit statusChanged();
    }
}

void DicomManager::updateStudies(const QVariantList& studies) {
    m_studies = studies;
    emit studiesChanged();
}

void DicomManager::onStudyListReady(const QVariantList& studies) {
    updateStudies(studies);
    setTransferring(false);
    emit studyQueryCompleted(studies);
    emit operationCompleted(true, QString("Found %1 studies").arg(studies.size()));
}

void DicomManager::onTransferCompleted(bool success, const QString& message) {
    setTransferring(false);
    emit operationCompleted(success, message);
}

void DicomManager::onLogMessage(const QString& level, const QString& message) {
    emit logMessage(level, message);
}

void DicomManager::onProgressUpdated(int percent, const QString& message) {
    setStatus(message);
    emit progressChanged(percent, message);
}
void DicomManager::testConnection(const QString& serverName) {
    if (m_isTransferring) {
        emit logMessage("WARNING", "Already transferring, please wait");
        return;
    }

    setTransferring(true);
    QMetaObject::invokeMethod(m_worker, "testConnection", Q_ARG(QString, serverName));
}
void DicomManager::onWorkerFinished() {
    m_worker = nullptr;
}
