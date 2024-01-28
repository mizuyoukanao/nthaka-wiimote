from Commands.Keys import Button, Direction, Hat
from Commands.PythonCommandBase import PythonCommand


class TestSerial(PythonCommand):
    NAME = "TestSerial"

    def __init__(self):
        super().__init__()

    def do(self):
        self.wait(1)

        self.press(Button.Y, duration=1, wait=1)
        self.press(Button.B, duration=1, wait=1)
        self.press(Button.A, duration=1, wait=1)
        self.press(Button.X, duration=1, wait=1)
        self.press(Button.L, duration=1, wait=1)
        self.press(Button.R, duration=1, wait=1)
        self.press(Button.ZL, duration=1, wait=1)
        self.press(Button.ZR, duration=1, wait=1)
        self.press(Button.MINUS, duration=1, wait=1)
        self.press(Button.PLUS, duration=1, wait=1)
        self.press(Button.LCLICK, duration=1, wait=1)
        self.press(Button.RCLICK, duration=1, wait=1)
        self.press(Button.HOME, duration=1, wait=1)
        self.press(Button.CAPTURE, duration=1, wait=1)

        self.press(Hat.TOP, duration=1, wait=1)
        self.press(Hat.TOP_RIGHT, duration=1, wait=1)
        self.press(Hat.RIGHT, duration=1, wait=1)
        self.press(Hat.BTM_RIGHT, duration=1, wait=1)
        self.press(Hat.BTM, duration=1, wait=1)
        self.press(Hat.BTM_LEFT, duration=1, wait=1)
        self.press(Hat.LEFT, duration=1, wait=1)
        self.press(Hat.TOP_LEFT, duration=1, wait=1)

        self.press(Direction.UP, duration=1, wait=1)
        self.press(Direction.UP_RIGHT, duration=1, wait=1)
        self.press(Direction.RIGHT, duration=1, wait=1)
        self.press(Direction.DOWN_RIGHT, duration=1, wait=1)
        self.press(Direction.DOWN, duration=1, wait=1)
        self.press(Direction.DOWN_LEFT, duration=1, wait=1)
        self.press(Direction.LEFT, duration=1, wait=1)
        self.press(Direction.UP_LEFT, duration=1, wait=1)

        self.press(Direction.R_UP, duration=1, wait=1)
        self.press(Direction.R_UP_RIGHT, duration=1, wait=1)
        self.press(Direction.R_RIGHT, duration=1, wait=1)
        self.press(Direction.R_DOWN_RIGHT, duration=1, wait=1)
        self.press(Direction.R_DOWN, duration=1, wait=1)
        self.press(Direction.R_DOWN_LEFT, duration=1, wait=1)
        self.press(Direction.R_LEFT, duration=1, wait=1)
        self.press(Direction.R_UP_LEFT, duration=1, wait=1)

        self.wait(1)
