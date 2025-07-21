import unittest
from app import app  # Make sure 'app' is the Flask instance in app.py

class FlaskAppTest(unittest.TestCase):
    def setUp(self):
        self.app = app.test_client()
        self.app.testing = True

    def test_hello_endpoint(self):
        response = self.app.get('/hello')
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.data.decode('utf-8'), 'Hello, World!')  # Or "Hello, PQC!" if updated

if __name__ == '__main__':
    unittest.main()
