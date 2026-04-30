import sys
import json
import contour_processor

class PythonBackend:
    def __init__(self):
        self.processor = contour_processor.ContourProcessor()
        self.current_contour = None

    def generate_contour(self, contour_type: str,
                        center_x: float,
                        center_y: float,
                        size: float) -> str:
        """Generate contour and return as JSON string"""
        try:
            contour_data = self.processor.generate_contour(
                contour_type, center_x, center_y, size
            )
            self.current_contour = contour_data
            return json.dumps(contour_data)
        except Exception as e:
            return json.dumps({"error": str(e)})

    def smooth_current_contour(self, smoothing: float) -> str:
        """Smooth the current contour"""
        if not self.current_contour:
            return json.dumps({"error": "No contour loaded"})

        try:
            smoothed_points = self.processor.smooth_contour(
                self.current_contour["points"], smoothing
            )
            self.current_contour["points"] = smoothed_points
            return json.dumps(self.current_contour)
        except Exception as e:
            return json.dumps({"error": str(e)})

    def calculate_area(self) -> float:
        """Calculate area of current contour"""
        if not self.current_contour:
            return 0.0

        return self.processor.calculate_area(self.current_contour["points"])
