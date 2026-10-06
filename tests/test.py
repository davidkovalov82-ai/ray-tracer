import os
import subprocess
import unittest

class TestRayTracer(unittest.TestCase):
    def setUp(self):
        # Компилируем проект, если бинарного файла нет
        if not os.path.exists('build/ray_tracer'):
            subprocess.run(['cmake', '-S', '.', '-B', 'build'], check=True)
            subprocess.run(['cmake', '--build', 'build'], check=True)
        
        # Удаляем старые изображения перед тестом
        ppm_path = 'images/ppm/image.ppm'
        png_path = 'images/png/image.png'
        if os.path.exists(ppm_path):
            os.remove(ppm_path)
        if os.path.exists(png_path):
            os.remove(png_path)
            
        # Запускаем программу
        result = subprocess.run(['./build/ray_tracer'], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, "Ray tracer execution failed")

    def test_ppm_generation(self):
        ppm_path = 'images/ppm/image.ppm'
        self.assertTrue(os.path.exists(ppm_path), "PPM file was not created")
        
        with open(ppm_path, 'r') as f:
            lines = f.readlines()
            
        self.assertGreaterEqual(len(lines), 3, "PPM file is too short")
        self.assertEqual(lines[0].strip(), "P3", "Invalid PPM magic number")
        self.assertEqual(lines[1].strip(), "256 256", "Invalid PPM dimensions")
        self.assertEqual(lines[2].strip(), "255", "Invalid PPM max color value")
        self.assertEqual(len(lines), 65536 + 3, "Invalid number of pixel lines")

    def test_png_generation(self):
        png_path = 'images/png/image.png'
        self.assertTrue(os.path.exists(png_path), "PNG file was not created by sips")

if __name__ == '__main__':
    unittest.main()
