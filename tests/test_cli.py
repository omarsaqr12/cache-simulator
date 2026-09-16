"""Black-box tests: execute the compiled CLI with controlled access traces."""
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]
EXE = ROOT / 'cache_simulator'


def run_cli(trace, config='4\n4\n1\n2\n'):
    with tempfile.TemporaryDirectory() as directory:
        path = pathlib.Path(directory) / 'trace.txt'
        path.write_text(trace, encoding='ascii')
        return subprocess.run([str(EXE)], input=config + str(path) + '\n',
                              capture_output=True, text=True, timeout=5)


class CacheCLITests(unittest.TestCase):
    def test_full_width_and_conflicts(self):
        trace = ('00000000000000000000000000000001I\n'
                 '00000000000000000000000000000001I\n'
                 '00000000000000000000000000000101I\n'
                 '00000000000000000000000000000001I\n'
                 '10000000000000000000000000000000D\n')
        outcome = run_cli(trace)
        self.assertEqual(outcome.returncode, 0, outcome.stderr)
        self.assertIn('Successful Hits: 1', outcome.stdout)
        self.assertIn('Misses: 3', outcome.stdout)
        self.assertIn('Average Memory Access Time: 77.00 cycles', outcome.stdout)
        self.assertIn('Cache Size: 4 bytes', outcome.stdout)

    def test_invalid_configuration_and_trace(self):
        self.assertNotEqual(run_cli('', '0\n4\n1\n2\n').returncode, 0)
        self.assertNotEqual(run_cli('', '4\n4\n3\n2\n').returncode, 0)
        malformed = run_cli('00000000000000000000000000000002I\n')
        self.assertNotEqual(malformed.returncode, 0)
        self.assertIn('Nonbinary address on line 1', malformed.stderr)
        short = run_cli('0000I\n')
        self.assertNotEqual(short.returncode, 0)
        self.assertIn('Malformed access on line 1', short.stderr)

    def test_empty_trace_has_defined_statistics(self):
        outcome = run_cli('')
        self.assertEqual(outcome.returncode, 0, outcome.stderr)
        self.assertEqual(outcome.stdout.count('Average Memory Access Time: 0.00 cycles'), 2)


if __name__ == '__main__':
    unittest.main()
