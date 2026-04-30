import numpy as np
from scipy import interpolate
import json
from typing import List, Tuple
import sys
import os

class ContourProcessor:
    def __init__(self):
        self.contours = []

    def generate_contour(self, contour_type: str,
                        center_x: float,
                        center_y: float,
                        size: float,
                        points: int = 100) -> dict:
        """Generate contour points"""

        t = np.linspace(0, 2 * np.pi, points)

        if contour_type == "circle":
            x = center_x + size * np.cos(t)
            y = center_y + size * np.sin(t)

        elif contour_type == "star":
            r = size * (0.8 + 0.2 * np.sin(5 * t))
            x = center_x + r * np.cos(t)
            y = center_y + r * np.sin(t)

        elif contour_type == "wave":
            t = np.linspace(0, 4 * np.pi, points)
            x = center_x + t / (4 * np.pi) * 2 * size - size
            y = center_y + size * 0.3 * np.sin(2 * t)

        elif contour_type == "heart":
            x = center_x + 16 * (np.sin(t) ** 3) * size / 16
            y = center_y - (13 * np.cos(t) - 5 * np.cos(2*t) -
                          2 * np.cos(3*t) - np.cos(4*t)) * size / 16

        else:
            # Default to circle
            x = center_x + size * np.cos(t)
            y = center_y + size * np.sin(t)

        # Convert to list
        points_list = [(float(x[i]), float(y[i])) for i in range(len(x))]

        return {
            "type": contour_type,
            "points": points_list,
            "centerX": float(center_x),
            "centerY": float(center_y),
            "size": float(size)
        }

    def smooth_contour(self, points: List[Tuple[float, float]],
                      smoothing: float = 0.1) -> List[Tuple[float, float]]:
        """Smooth contour using spline interpolation"""
        if len(points) < 4:
            return points

        points_array = np.array(points)
        x = points_array[:, 0]
        y = points_array[:, 1]

        # Create spline
        tck, u = interpolate.splprep([x, y], s=smoothing)

        # Generate smoothed points
        u_new = np.linspace(0, 1, len(x) * 2)
        x_new, y_new = interpolate.splev(u_new, tck)

        return [(float(x_new[i]), float(y_new[i])) for i in range(len(x_new))]

    def offset_contour(self, points: List[Tuple[float, float]],
                      offset_x: float,
                      offset_y: float) -> List[Tuple[float, float]]:
        """Offset contour points"""
        return [(x + offset_x, y + offset_y) for x, y in points]

    def calculate_area(self, points: List[Tuple[float, float]]) -> float:
        """Calculate area of contour using shoelace formula"""
        if len(points) < 3:
            return 0.0

        x = [p[0] for p in points]
        y = [p[1] for p in points]

        area = 0.5 * abs(sum(x[i] * y[i+1] - x[i+1] * y[i]
                           for i in range(len(points)-1)))
        return float(area)
