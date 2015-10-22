class Desk(object):
    """docstring for Desk"""

    TOP = 1
    LEFT = 2
    BOTTOM = 3
    RIGHT = 4

    def __init__(self, width, height, matrix, blank_x, blank_y):
        self.width = width
        self.height = height
        self.matrix = matrix[:]
        self.blank_x = blank_x
        self.blank_y = blank_y

    def change_neighbour(self, position):
        x = self.blank_x
        y = self.blank_y
        if position == self.TOP:
            if x == 0:
                return False
            
            k = self.matrix[x][y]
            self.matrix[x][y] = self.matrix[x-1][y]
            self.matrix[x-1][y] = k
            self.blank_x = x - 1

        if position == self.RIGHT:
            if y == self.height - 1:
                return False

            k = self.matrix[x][y]
            self.matrix[x][y] = self.matrix[x][y+1]
            self.matrix[x][y+1] = k
            self.blank_y = y + 1

        if position == self.BOTTOM:
            if x == self.width - 1:
                return False

            k = self.matrix[x][y]
            self.matrix[x][y] = self.matrix[x+1][y]
            self.matrix[x+1][y] = k
            self.blank_x = x + 1

        if position == self.LEFT:
            if y == 0:
                return False

            k = self.matrix[x][y]
            self.matrix[x][y] = self.matrix[x][y-1]
            self.matrix[x][y-1] = k
            self.blank_y = y - 1

        return True