#include "qa.h"
#include "ui_qa.h"

QA::QA(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QAClass)
{
    ui->setupUi(this);
    connect(this, SIGNAL(signalAppendToOutput(QString)), ui->object, SLOT(append(QString)));
    connect(this, SIGNAL(signalUpdateProgressbar(int)), ui->bar, SLOT(setValue(int)));

}

QA::~QA()
{
    delete ui;
}

Ui::QAClass *global_ui;
QA *global_qa;

void* qa_check(void*);

void QA::on_check_clicked()
{
    while (global_ui)
        usleep(100);
    global_ui = ui;
    global_qa = this;

    global_ui->object->setHtml(QString());

    pthread_t thread;
    pthread_create (&thread, NULL, qa_check, (void*)(strdup(global_ui->input->toPlainText().toStdString().c_str())));
}

void* qa_check(void* plainText) {
    QStringList listOfSentences = QString((char*)plainText).split("\n", QString::SkipEmptyParts);

    char* base_dir = strdup(".");
    char* language = strdup(global_ui->language->currentText().toStdString().c_str());
    char* programming_language = strdup(global_ui->programming_language->currentText().toStdString().c_str());
    if (!language || !global_ui->language->currentText().toStdString().size())
        strcpy(language, "de");
    if (!programming_language || !global_ui->programming_language->currentText().toStdString().size())
        strcpy(programming_language, "perl5");

    {
        char* sqlite_filename = (char*)calloc(512 + 1, 1);
        strcat(sqlite_filename, base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, language);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }
    hal2009_clean();

    fprintf(stdout, "Programming language is %s\n", programming_language);
    fprintf(stdout, "Text language is %s\n", language);
    pthread_t signal_thread = hal2009_start_signal_handler(strdup(programming_language), strdup(language), MULTI);
    fprintf(stdout, "Shell started...\nThis is %s.\n\n", FULL_NAME);

    emit global_qa->signalUpdateProgressbar((int)(0.0));
    for (int i = 0; i < listOfSentences.size(); ++i) {
        if (listOfSentences.at(i).toStdString().size() == 0)
            continue;

        cout << "\nTesting: " << listOfSentences.at(i).toStdString() << "...\n" << endl;
        emit global_qa->signalAppendToOutput(QString("<i>User:</i>&nbsp;&nbsp;&nbsp; ") + listOfSentences.at(i));

        char* copy_of_programming_language = (char*)cxxhalmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_programming_language, programming_language);
        char* copy_of_language             = (char*)cxxhalmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_language,             language);
        char* copy_of_base_dir             = (char*)cxxhalmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_base_dir,             base_dir);
        char* copy_of_input                = strdup(listOfSentences.at(i).toStdString().c_str());
        pthread_t answer_thread = hal2009_answer(copy_of_input, copy_of_programming_language, copy_of_language, copy_of_base_dir, NOT_JOIN, MULTI);
        pthread_join(answer_thread, NULL);

        emit global_qa->signalUpdateProgressbar((int)(100.0/listOfSentences.size()*(i+1)));
        usleep(1000);
    }

    emit global_qa->signalUpdateProgressbar((int)(100.0));
    usleep(10000);
    emit global_qa->signalUpdateProgressbar((int)(0.0));

    global_ui = 0;
    free(plainText);
}

void hal2009_handle_signal(void* arg) {
    char* type       = (char*)((void**)arg)[0];
    char* text       = (char*)((void**)arg)[1];
    char* start_type = (char*)((void**)arg)[2];

    if (0 == strcmp(type, "_output__pos")) {
        printf("\nUnknown part of speech: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        FILE* target = fopen("_input__pos", "w+b");
        fwrite(text, 1, strlen(text), target);
        fclose(target);
    }
    else if (0 == strcmp(type, "_output__link")) {
        if (strlen(text) < 99) {
            char link[99] = {0};
            int f1 = 0;
            int f2 = 0;
            sscanf(text, "%98[a-zA-Z]%d\n%d", &link, &f1, &f2);
            printf("--%s--\n%s\n%i\n%i", text, link, f1, f2);
            hal2009_add_link(link, f1, f2);
        }
    }
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        hal2009_add_pro_file(text);
        FILE* target = fopen("_input__add_pro_file", "w+b");
        fclose(target);
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        struct DATASET set = hal2009_get_csv(text);
        FILE* target = fopen("_input__get_csv", "w+b");
        const char* csv_data = hal2009_make_csv(&set);
        fwrite(csv_data, 1, strlen(csv_data), target);
        fclose(target);
        printf("Release memory now.\n");
        free((void*)csv_data);
        printf("Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        printf("\nFreeHAL: %s\n", text);
        emit global_qa->signalAppendToOutput(QString("<i>FreeHAL:</i> ") + QString::fromStdString(std::string(text)));
    }
    else if (0 == strcmp(type, "_exit") && (long)start_type == SINGLE) {
        exit(0);
    }
}
