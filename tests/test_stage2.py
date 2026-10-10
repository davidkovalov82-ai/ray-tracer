"""Integration checks for the current 256 x 256 stage-2 background.

Run from the project root: python3 tests/test_stage2.py --binary build/ray_tracer
Uses only the Python standard library. Does not overwrite existing images.
"""

import argparse
from pathlib import Path
import subprocess
import tempfile
import unittest


class Stage2ImageTests(unittest.TestCase):
    binary = None

    @classmethod
    def setUpClass(cls):
        # Изолированный запуск сохраняет существующие изображения проекта.
        with tempfile.TemporaryDirectory(prefix="ray-tracer-stage2-") as folder:
            root = Path(folder)
            (root / "images/ppm").mkdir(parents=True)
            (root / "images/png").mkdir(parents=True)
            run = subprocess.run(
                [str(cls.binary)], cwd=root, capture_output=True, text=True, timeout=30
            )
            if run.returncode != 0:
                raise RuntimeError(f"Renderer exited with {run.returncode}\n{run.stdout}\n{run.stderr}")
            path = root / "images/ppm/image.ppm"
            if not path.is_file():
                raise RuntimeError(f"Renderer did not create image.ppm\n{run.stdout}\n{run.stderr}")
            # P3 поддерживает комментарии после # и произвольные пробелы.
            tokens = [
                token
                for line in path.read_text(encoding="ascii").splitlines()
                for token in line.split("#", 1)[0].split()
            ]
        if len(tokens) < 4 or tokens[0] != "P3":
            raise RuntimeError("Expected a text P3 PPM with a complete header")
        cls.width, cls.height, cls.maximum = map(int, tokens[1:4])
        cls.channels = [int(value) for value in tokens[4:]]

    def pixel(self, x, y):
        index = 3 * (y * self.width + x)
        pixel = tuple(self.channels[index:index + 3])
        self.assertEqual(len(pixel), 3, f"Missing pixel ({x}, {y})")
        return pixel

    def expect_pixel(self, x, y, expected):
        # Допуск 2 уровня из 255 учитывает округление при записи PPM.
        for actual, target in zip(self.pixel(x, y), expected):
            self.assertLessEqual(abs(actual - target), 2, f"Pixel ({x}, {y})")

    def test_header(self):
        self.assertEqual((self.width, self.height, self.maximum), (256, 256, 255))

    def test_pixel_count(self):
        self.assertEqual(len(self.channels), self.width * self.height * 3)

    def test_channel_range(self):
        self.assertTrue(all(0 <= value <= 255 for value in self.channels))

    def test_blue_channel(self):
        # У обоих цветов blue = 1, поэтому синий канал неизменен.
        self.assertTrue(self.channels)
        self.assertTrue(all(value == 255 for value in self.channels[2::3]))

    def test_top_middle_bottom_colors(self):
        self.expect_pixel(128, 0, (146, 189, 255))
        self.expect_pixel(128, 127, (191, 216, 255))
        self.expect_pixel(128, 255, (236, 243, 255))

    def test_vertical_transition(self):
        previous = self.pixel(128, 0)
        for y in range(1, self.height):
            current = self.pixel(128, y)
            self.assertGreaterEqual(current[0], previous[0], f"Red decreases at row {y}")
            self.assertGreaterEqual(current[1], previous[1], f"Green decreases at row {y}")
            previous = current

    def test_horizontal_symmetry_and_ray_direction(self):
        for y in (0, 64, 127, 192, 255):
            for x in range(self.width // 2):
                left = self.pixel(x, y)
                right = self.pixel(self.width - 1 - x, y)
                self.assertTrue(all(abs(a - b) <= 1 for a, b in zip(left, right)))
        # Нормализация направления создаёт небольшую кривизну градиента.
        # Прямой градиент только от номера строки эту проверку не пройдёт.
        self.assertGreater(self.pixel(0, 0)[0], self.pixel(128, 0)[0])
        self.assertLess(self.pixel(0, 255)[0], self.pixel(128, 255)[0])


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, default=Path("build/ray_tracer"))
    args = parser.parse_args()
    Stage2ImageTests.binary = args.binary.resolve()
    if not Stage2ImageTests.binary.is_file():
        parser.error("Renderer not found; run make build first")
    unittest.main(argv=["test_stage2.py"], verbosity=2)
