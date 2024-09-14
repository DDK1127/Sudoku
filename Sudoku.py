import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QLineEdit, QPushButton, QMessageBox
from PyQt5.QtCore import Qt, QTimer

class Sudoku:
    def __init__(self, grid=None):
        self.grid = grid if grid else [[0] * 9 for _ in range(9)]
        self.find = False

    def input(self, grid):
        self.grid = grid
        self.find = False

    def possible_list(self, row, col):
        possible = set(range(1, 10))
        possible -= set(self.grid[row])
        possible -= set(self.grid[i][col] for i in range(9))
        start_row, start_col = 3 * (row // 3), 3 * (col // 3)
        for i in range(start_row, start_row + 3):
            for j in range(start_col, start_col + 3):
                possible.discard(self.grid[i][j])
        return list(possible)

    def next_location(self, row, col):
        while row < 9:
            if col > 8:
                row += 1
                col = 0
            elif self.grid[row][col] == 0:
                return row, col
            else:
                col += 1
        return None, None

    def dfs(self, row, col):
        if row == 9:
            self.find = True
            return True
        if self.grid[row][col] != 0:
            if col == 8:
                if self.dfs(row + 1, 0):
                    return True
            else:
                if self.dfs(row, col + 1):
                    return True
        else:
            for num in self.possible_list(row, col):
                self.grid[row][col] = num
                if col == 8:
                    if self.dfs(row + 1, 0):
                        return True
                else:
                    if self.dfs(row, col + 1):
                        return True
                self.grid[row][col] = 0
        return False

class SudokuGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.sudoku = Sudoku()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Sudoku Solver')
        self.setFixedSize(380, 420)  # 設置固定窗口大小

        self.grid_layout = QGridLayout()
        self.grid_layout.setSpacing(0)  # 取消網格之間的空隙
        self.entries = [[QLineEdit(self) for _ in range(9)] for _ in range(9)]
        for i in range(9):
            for j in range(9):
                self.entries[i][j].setFixedSize(40, 40)
                self.entries[i][j].setAlignment(Qt.AlignCenter)
                self.entries[i][j].setMaxLength(1)  # 設置每個輸入框只能輸入一個字符
                self.entries[i][j].setStyleSheet("background-color: #f5f5dc; color: black; border: 1px solid gray;")  # 米白色背景，黑色輸入

                # 添加粗邊框來標識九宮格區塊
                if i % 3 == 0:
                    self.entries[i][j].setStyleSheet(self.entries[i][j].styleSheet() + "border-top: 2px solid black;")
                if j % 3 == 0:
                    self.entries[i][j].setStyleSheet(self.entries[i][j].styleSheet() + "border-left: 2px solid black;")
                if i == 8:
                    self.entries[i][j].setStyleSheet(self.entries[i][j].styleSheet() + "border-bottom: 2px solid black;")
                if j == 8:
                    self.entries[i][j].setStyleSheet(self.entries[i][j].styleSheet() + "border-right: 2px solid black;")

                self.grid_layout.addWidget(self.entries[i][j], i, j)

        self.solve_button = QPushButton('Solve', self)
        self.solve_button.clicked.connect(self.solve_sudoku)
        self.grid_layout.addWidget(self.solve_button, 9, 0, 1, 9)

        self.setLayout(self.grid_layout)

        # 設置鍵盤事件處理
        self.setFocusPolicy(Qt.StrongFocus)

        self.solve_button = QPushButton('Solve', self)
        self.solve_button.clicked.connect(self.solve_sudoku)
        self.grid_layout.addWidget(self.solve_button, 9, 0, 1, 9)

        self.setLayout(self.grid_layout)

        # 設置鍵盤事件處理
        self.setFocusPolicy(Qt.StrongFocus)
    def keyPressEvent(self, event):
        current_widget = self.focusWidget()
        if isinstance(current_widget, QLineEdit):
            index = None
            for i in range(9):
                for j in range(9):
                    if self.entries[i][j] == current_widget:
                        index = (i, j)
                        break
                if index:
                    break
                    if event.key() == Qt.Key_Return or event.key() == Qt.Key_Enter:
                        self.solve_sudoku()
            if index:
                i, j = index
                if event.key() == Qt.Key_Left:
                    if j > 0:
                        self.entries[i][j - 1].setFocus()
                elif event.key() == Qt.Key_Right:
                    if j < 8:
                        self.entries[i][j + 1].setFocus()
                elif event.key() == Qt.Key_Up:
                    if i > 0:
                        self.entries[i - 1][j].setFocus()
                elif event.key() == Qt.Key_Down:
                    if i < 8:
                        self.entries[i + 1][j].setFocus()

    def get_grid(self):
        for i in range(9):
            for j in range(9):
                try:
                    value = int(self.entries[i][j].text())
                    self.sudoku.grid[i][j] = value if value in range(1, 10) else 0
                except ValueError:
                    self.sudoku.grid[i][j] = 0

    def display_solution(self):
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_grid)
        self.timer.start(100)

    def update_grid(self):
        for i in range(9):
            for j in range(9):
                if self.entries[i][j].text() == '' or self.entries[i][j].text() == '0':
                    self.entries[i][j].setText(str(self.sudoku.grid[i][j]))
                    self.entries[i][j].setStyleSheet("background-color: #d0f0c0; color: black; border: 1px solid gray;")  # 淡綠色背景，黑色輸入
                    return
        self.timer.stop()
        QMessageBox.information(self, "Sudoku Solver", "Solution found!")

    def solve_sudoku(self):
        self.get_grid()
        self.sudoku.dfs(0, 0)
        if self.sudoku.find:
            self.display_solution()
        else:
            QMessageBox.critical(self, "Sudoku Solver", "No solution exists.")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = SudokuGUI()
    ex.show()
    sys.exit(app.exec_())