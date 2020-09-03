import time


class FPS:
    def __init__(self):
        self.beg = 0
        self.avgFPS = 0
        self.fps1sec = 0

    def calcFPS(self):
        if time.time() - self.beg > 1:
            self.beg = time.time()
            self.avgFPS = 0.7 * self.avgFPS + 0.3 * self.fps1sec
            self.fps1sec = 0

        self.fps1sec += 1
        return self.avgFPS
1