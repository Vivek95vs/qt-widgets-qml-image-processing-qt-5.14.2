#include "Program.h"
#include "Client.h"
#include "Protocol.h"

Program::PatientOrientation_Standing Program::targetpos_cal_PatientStanding = Program::PatientOrientation_Standing::patientOrientationstandNULL;

Program::Program(QObject *parent) : QObject(parent)
{
}

void Program::Main()
{
    Protocol::CreateWorkflowLogfile(Protocol::WorkdflowfilePath, Protocol::WorkdflowfileName);

    Client client;
    Protocol m_procol;
    client.ConnectStatus = true;
    client.LPC_connect(client.IP_Address, client.PortNumber);
    m_procol.BootParameter();
    Protocol::DRReq_ThreadStart();

}
