#include "converterhex.h"
#include "ui_converterhex.h"

QString fileName;
QString binFileName;
int mem_size;

FILE *fin; // Глобальная переменная для дескриптора hex файла
FILE *fout; // Глобальная переменная для дескриптора bin файла

unsigned char hex_to_byte(char c1, char c2);
void hex_to_bin(FILE *hex_file, FILE *bin_file, int mem_size); // Изменили прототип функции

ConverterHex::ConverterHex(QWidget *parent) : QMainWindow(parent), ui(new Ui::ConverterHex) {
  ui->setupUi(this);
  ui->textEdit->setDisabled(true);
  ui->labelSizeFlash->setValidator(new QIntValidator);
  //connect(ui->ButOpenFile, SIGNAL(clicked()), this, SLOT(on_ButOpenFile_clicked()));
  //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
}

ConverterHex::~ConverterHex() {
  delete ui;
}


void ConverterHex::on_ButOpenFile_clicked()
{
    // Выбираем hex файл для открытия
    fileName = QFileDialog::getOpenFileName(this, "Выбрать файл", "C:\\", "Hex File (*.hex);; All Files (*.*);;");
    ui->textEdit->setText(fileName);
    // Открываем hex файл для чтения
    fin = fopen(fileName.toStdString().c_str(), "r");
}

void ConverterHex::on_pushButton_clicked()
{
    // Получаем размер памяти из метки
    mem_size = ui->labelSizeFlash->text().toInt();

    // Выбираем bin файл для сохранения
    binFileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "C:\\", "Bin File (*.bin);; All Files (*.*);;");

    // Открываем bin файл для записи
    fout = fopen(binFileName.toStdString().c_str(), "wb");

    // Конвертируем hex файл в bin файл
    hex_to_bin(fin, fout, mem_size); // Передаем дескрипторы файлов в функцию

    // Закрываем файлы
    fclose(fin);
    fclose(fout);
}

// Функция, которая конвертирует два символа hex в один байт
unsigned char hex_to_byte(char c1, char c2) {
  unsigned char b = 0;
  if (c1 >= '0' && c1 <= '9') {
    b = (c1 - '0') << 4;
  } else if (c1 >= 'A' && c1 <= 'F') {
    b = (c1 - 'A' + 10) << 4;
  } else if (c1 >= 'a' && c1 <= 'f') {
    b = (c1 - 'a' + 10) << 4;
  } else {
    return 0;
  }
  if (c2 >= '0' && c2 <= '9') {
    b += c2 - '0';
  } else if (c2 >= 'A' && c2 <= 'F') {
    b += c2 - 'A' + 10;
  } else if (c2 >= 'a' && c2 <= 'f') {
    b += c2 - 'a' + 10;
  } else {
    return 0;
  }
  return b;
}

// Функция, которая конвертирует hex файл в bin файл
void hex_to_bin(FILE *hex_file, FILE *bin_file, int mem_size) { // Изменили тип первого аргумента функции
  char line[256];
  int byte_count, address, record_type;
  unsigned char checksum, data[256];
  int i, j;

  // Заполняем bin файл байтами 0xff
  for (i = 0; i < mem_size * 1024; i++) {
    fputc(0xff, bin_file);
  }

  // Читаем строки из hex файла
  while (fgets(line, sizeof(line), hex_file)) {
    // Проверяем, что строка начинается с ':'
    if (line[0] != ':') {
      continue;
    }
    // Получаем количество байтов, адрес, тип записи и контрольную сумму из строки
    byte_count = hex_to_byte(line[1], line[2]);
    address = hex_to_byte(line[3], line[4]) << 8 | hex_to_byte(line[5], line[6]);
    record_type = hex_to_byte(line[7], line[8]);
    checksum = hex_to_byte(line[9 + byte_count * 2], line[10 + byte_count * 2]);
    // Получаем данные из строки
    for (i = 0; i < byte_count; i++) {
      data[i] = hex_to_byte(line[9 + i * 2], line[10 + i * 2]);
    }
    // Если тип записи равен 0, то записываем данные в bin файл по адресу
    if (record_type == 0) {
      fseek(bin_file, address, SEEK_SET);
      fwrite(data, 1, byte_count, bin_file);
    }
    // Если тип записи равен 1, то завершаем обработку hex файла
    if (record_type == 1) {
      break;
    }
  }
}
