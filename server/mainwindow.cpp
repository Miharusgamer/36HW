#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow),
    activeUserRow_(0), activeMessageRow_(0)
{
    ui->setupUi(this);

    database_ = new Database;

    modelUsers_ = new QSqlTableModel(this, *database_->getDatabase());
    modelMessages_ = new QSqlTableModel(this, *database_->getDatabase());
    modelUsers_->setTable("Users");
    modelMessages_->setTable("Messages");
    modelUsers_->select();
    modelMessages_->select();

    ui->tableView_Users->setModel(modelUsers_);
    ui->tableView_Messages->setModel(modelMessages_);

    const uint16_t port = 7777;
    server_ = new Server(port, database_);

    connect(database_, &Database::updateDatabase, this, &MainWindow::updateViews);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_RemoveUser_clicked()
{
    auto idCell = ui->tableView_Users->model()->index(activeUserRow_, 0);
    auto loginToDelete = ui->tableView_Users->model()->data(idCell).toString();

    QDialog dialog(this);
    dialog.setModal(true);
    auto layout = new QVBoxLayout();
    dialog.setLayout(layout);

    auto label = new QLabel(&dialog);
    QString message = "Are you sure you want to remove user: " + loginToDelete + "?";
    label->setText(message);
    label->setAlignment(Qt::AlignHCenter);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, &dialog);

    layout->setAlignment(buttonBox, Qt::AlignHCenter);

    auto splitter = new QSplitter(&dialog);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(label);
    splitter->addWidget(buttonBox);
    layout->addWidget(splitter);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    auto result = dialog.exec();
    if (result == QDialog::Accepted){
        database_->removeUser(loginToDelete.toStdString());
        activeUserRow_ = 0;
    }
}

void MainWindow::on_pushButton_RemoveMessage_clicked()
{
    auto idCell = ui->tableView_Messages->model()->index(activeMessageRow_, 2);
    auto messageText = ui->tableView_Messages->model()->data(idCell).toString();

    QDialog dialog(this);
    dialog.setModal(true);
    auto layout = new QVBoxLayout();
    dialog.setLayout(layout);

    auto label = new QLabel(&dialog);
    QString message = "Are you sure you want to remove message: '" + messageText + "'?";
    label->setText(message);
    label->setAlignment(Qt::AlignHCenter);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, &dialog);

    layout->setAlignment(buttonBox, Qt::AlignHCenter);

    auto splitter = new QSplitter(&dialog);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(label);
    splitter->addWidget(buttonBox);
    layout->addWidget(splitter);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    auto result = dialog.exec();
    if (result == QDialog::Accepted){
        database_->removeMessage(messageText.toStdString());
        activeMessageRow_ = 0;
    }
}

void MainWindow::on_tableView_Users_clicked(const QModelIndex &index)
{
    activeUserRow_ = index.row();
}

void MainWindow::on_tableView_Users_doubleClicked()
{
    auto idCell = ui->tableView_Users->model()->index(activeUserRow_, 0);
    auto login = ui->tableView_Users->model()->data(idCell).toString();
    database_->switchBanUser(login.toStdString());
}

void MainWindow::on_tableView_Messages_clicked(const QModelIndex &index)
{
    activeMessageRow_ = index.row();
}

void MainWindow::updateViews()
{
    modelUsers_->select();
    modelMessages_->select();
}
