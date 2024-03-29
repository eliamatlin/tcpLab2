#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::NoContextMenu);
    setFixedSize(633,383);
    ui->text_show_elements->setStyleSheet("font: 12pt");
    ui->lineEditElement->setStyleSheet("font: 12pt");
    ui->lineEditElement->setValidator(new QIntValidator);
    initialization_item();
    hideAllObject();

}

MainWindow::~MainWindow()
{
    bn.delete_tree_r(&root);
    delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{


    QString filePath = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "*.txt");
    if(filePath.isEmpty()) return;

    bn.results.clear();
    bn.delete_tree_r(&root);

    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    strNumbers = in.readLine();

    file.close();

    for(const auto &s : strNumbers.split(" "))
        queue_numbers_to_tree.push(s.toInt());


    while (!queue_numbers_to_tree.empty())
    {
       bn.search_insert_r(&root, queue_numbers_to_tree.front());
       queue_numbers_to_tree.pop();
    }


    bn.print_tree(root,"");

    ui->text_show_elements->setText(strNumbers);

    hideAllObject();
    updateWindow();
}


void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    std::map<std::string,int>::iterator it;
    for(int i = 0; i < 15; i++)
    {
        it = bn.results.find(ID_LABELS[i]);
        if(it != bn.results.end())
        {
             painter.drawLine(lines_to_ellipse[ID_LABELS[i]]);
        }
    }

}


void MainWindow::updateWindow()
{
    hideAllObject();

    QString temp_str, str_for_show;
    std::map<std::string,int>::iterator it;
    for(int i = 0; i < 15; i++)
    {
        it = bn.results.find(ID_LABELS[i]);
        if(it != bn.results.end())
        {
            temp_str = QString::number(it->second);
            arrayNumberLabels[i].first->setVisible(true);
            arrayNumberLabels[i].first->setText(temp_str);
            arrayNumberLabels[i].second->setVisible(true);
            str_for_show.append(temp_str).append(" ");
        }
    }


    ui->text_show_elements->setText(str_for_show);
    update();
}

void MainWindow::on_buttonAdd_clicked()
{
    if(ui->lineEditElement->text().isEmpty())
        return;

    bn.search_insert_r(&root, ui->lineEditElement->text().toInt());
    bn.results.clear();
    bn.print_tree(root,"");
    updateWindow();
}

void MainWindow::on_buttonDelete_clicked()
{
    if(ui->lineEditElement->text().isEmpty())
        return;

    if(!bn.delete_node(&root, ui->lineEditElement->text().toInt()))
        QMessageBox::information(0, "Внимание!", "Данного элемента не существует в дереве.");

    bn.results.clear();
    bn.print_tree(root,"");
    updateWindow();
}

void MainWindow::hideAllObject()
{
    for(std::pair<QLabel*,QLabel*>  &p : arrayNumberLabels){
        p.first->setVisible(false); p.second->setVisible(false);
    }
}


void MainWindow::on_actionSave_File_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                QString::fromUtf8("Сохранить файл"),
                                QDir::currentPath(),
                                ".txt");
    if(filePath.isEmpty()) return;

    QFile file(filePath.append(".txt"));

    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    //QTextStream out(&file); // Открываем поток для ввода в файл


    //for (std::map<std::string,int>::iterator &it = bn.results.begin(); it!= bn.results.end(); ++it){ // переписываем элементы с контейнера в сохраненный файл
        //out << it->second; if ( std::next(it, 1) !=  bn.results.end())  out << " "; }

    //file.close();

}

void MainWindow::initialization_item()
{
    using std::make_pair;

    arrayNumberLabels.append(make_pair(ui->node_C, ui->e_node_C));
    arrayNumberLabels.append(make_pair(ui->node_L, ui->e_node_L));
    arrayNumberLabels.append(make_pair(ui->node_LL, ui->e_node_LL));
    arrayNumberLabels.append(make_pair(ui->node_LLL, ui->e_node_LLL));
    arrayNumberLabels.append(make_pair(ui->node_LLR, ui->e_node_LLR));
    arrayNumberLabels.append(make_pair(ui->node_LRL, ui->e_node_LRL));
    arrayNumberLabels.append(make_pair(ui->node_LRR, ui->e_node_LRR));
    arrayNumberLabels.append(make_pair(ui->node_LR, ui->e_node_LR));
    arrayNumberLabels.append(make_pair(ui->node_R, ui->e_node_R));
    arrayNumberLabels.append(make_pair(ui->node_RR, ui->e_node_RR));
    arrayNumberLabels.append(make_pair(ui->node_RRR, ui->e_node_RRR));
    arrayNumberLabels.append(make_pair(ui->node_RL, ui->e_node_RL));
    arrayNumberLabels.append(make_pair(ui->node_RLR, ui->e_node_RLR));
    arrayNumberLabels.append(make_pair(ui->node_RLL, ui->e_node_RLL));
    arrayNumberLabels.append(make_pair(ui->node_RRL, ui->e_node_RRL));


    lines_to_ellipse["L"] = QLine(316,102,245,131);
    lines_to_ellipse["LL"] = QLine(211,146,176,177);
    lines_to_ellipse["LLL"] = QLine(150,205,132,244);
    lines_to_ellipse["LLR"] = QLine(170,209,177,243);
    lines_to_ellipse["LR"] = QLine(242,154,266,178);
    lines_to_ellipse["LRL"] = QLine(265,204,252,242);
    lines_to_ellipse["LRR"] = QLine(289,207,305,243);
    lines_to_ellipse["R"] = QLine(354,101,432,133);
    lines_to_ellipse["RL"] = QLine(434,152,414,176);
    lines_to_ellipse["RLL"] = QLine(393,206,378,244);
    lines_to_ellipse["RLR"] = QLine(416,206,432,242);
    lines_to_ellipse["RR"] = QLine(464,153,491,175);
    lines_to_ellipse["RRL"] = QLine(498,209,491,243);
    lines_to_ellipse["RRR"] = QLine(514,206,536,243);

    ID_LABELS[0] = "";     ID_LABELS[1] = "L";
    ID_LABELS[2] = "LL";   ID_LABELS[3] = "LLL";
    ID_LABELS[4] = "LLR";  ID_LABELS[5] = "LRL";
    ID_LABELS[6] = "LRR";  ID_LABELS[7] = "LR";
    ID_LABELS[8] = "R";    ID_LABELS[9] = "RR";
    ID_LABELS[10] = "RRR"; ID_LABELS[11] = "RL";
    ID_LABELS[12] = "RLR"; ID_LABELS[13] = "RLL";
    ID_LABELS[14] = "RRL";

}


