#include "heartanalysispanel.h"
#include "ui_heartanalysispanel.h"
#include "setting_defines.h"

#include <QDebug>

HeartAnalysisPanel::HeartAnalysisPanel(
                    FftControl *fftControl,
                    QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeartAnalysisPanel)
{
    _fftControl = fftControl;

    ui->setupUi(this);

    connect(ui->cbAnalysis, SIGNAL(clicked(bool)),
            this, SLOT(onButtonAnalyzeState(bool)));

    connect(ui->pbManual, SIGNAL(clicked(bool)),
            this, SLOT(onButtonAnalyze(bool)));

    connect(ui->pbClear, SIGNAL(clicked(bool)),
            this, SLOT(onButtonClose(bool)));

    // Set hint
    ui->bpmB->setToolTip("Heart rate - the number of heart beats per minute");
    ui->bpmF->setToolTip("Heart rate - the number of heart beats per minute");
    ui->ibiB->setToolTip("The interval between heartbeats");
    ui->ibiF->setToolTip("The interval between heartbeats");
    ui->sdnnB->setToolTip("Standard deviation of RR intervals");
    ui->sdnnF->setToolTip("Standard deviation of RR intervals");
    ui->sdsdB->setToolTip("Standard deviation of successive differences");
    ui->sdsdF->setToolTip("Standard deviation of successive differences");
    ui->rmssdB->setToolTip("RMS of consecutive differences");
    ui->rmssdF->setToolTip("RMS of consecutive differences");
    ui->hrMadB->setToolTip("Median absolute deviation of RR intervals");
    ui->hrMadF->setToolTip("Median absolute deviation of RR intervals");
    ui->brethB->setToolTip("Respiratory rate");
    ui->brethF->setToolTip("Respiratory rate");

    // Python activation
    pyInstance = new CPyInstance();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(os.getcwd())");

    PyObject *module = PyImport_ImportModule("python_modules.biosignal_analysis");
    Py_DECREF(module);
}

HeartAnalysisPanel::~HeartAnalysisPanel()
{
    delete ui;
    delete pyInstance;
}

void HeartAnalysisPanel::makeAnalysis()
{
    PyObject *moduleB, *pythonClassB, *objectB, *argsB, *methodB, *calcB;
    PyObject *moduleF, *pythonClassF, *objectF, *argsF, *methodF, *calcF;

    // ----------------- Create class object for Basic signal  -------------------
    // Load the module object
    moduleB = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (moduleB == nullptr) {
        PyErr_Print();
    }
    // Builds the name of a callable class
    pythonClassB = PyObject_GetAttrString(moduleB, "HeartAnalysis");
    if (pythonClassB == nullptr) {
        PyErr_Print();
    }

    argsB  = Py_BuildValue("(fs)", static_cast<float>(_fftControl->getSamplingFreq()), "Basic_signal");
    // Creates an instance of the class
    objectB = PyObject_CallObject(pythonClassB, argsB);
    if (objectB == nullptr) {
        PyErr_Print();
    }

    // --------------- Create class object for Filtered signal  ----------------
    // Load the module object
    moduleF = PyImport_ImportModule("python_modules.biosignal_analysis");
    if (moduleF == nullptr) {
        PyErr_Print();
    }
    // Builds the name of a callable class
    pythonClassF = PyObject_GetAttrString(moduleF, "HeartAnalysis");
    if (pythonClassF == nullptr) {
        PyErr_Print();
    }

    argsF  = Py_BuildValue("(fs)", static_cast<float>(_fftControl->getSamplingFreq()), "Filtered_signal");
    // Creates an instance of the class
    objectF = PyObject_CallObject(pythonClassF, argsF);
    if (objectF == nullptr) {
        PyErr_Print();
    }
    // ----------------- Get Basic BPM -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_bpm");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double bpmBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic IBI -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_ibi");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double ibiBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic SDNN -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_sdnn");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdnnBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic SDSD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_sdsd");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdsdBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic RMSSD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_rmssd");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double rmssdBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic HR_MAD -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_hr_mad");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double hrMadBasic = PyFloat_AsDouble(calcB);
    // ----------------- Get Basic Breathingrate -------------------
    // Get class method
    methodB = PyObject_GetAttrString(objectB, "get_breathingrate");
    if (methodB == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcB = PyObject_CallObject(methodB, NULL);
    if (calcB == nullptr) {
        PyErr_Print();
    }
    // Get result
    double breathingrateBasic = PyFloat_AsDouble(calcB);

    // ----------------- Get Filtered BPM -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_bpm");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double bpmFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered IBI -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_ibi");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double ibiFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered SDNN -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_sdnn");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdnnFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered SDSD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_sdsd");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double sdsdFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered RMSSD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_rmssd");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double rmssdFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered HR_MAD -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_hr_mad");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double hrMadFiltered = PyFloat_AsDouble(calcF);
    // ----------------- Get Filtered Breathingrate -------------------
    // Get class method
    methodF = PyObject_GetAttrString(objectF, "get_breathingrate");
    if (methodF == nullptr) {
        PyErr_Print();
    }
    // Execute method
    calcF = PyObject_CallObject(methodF, NULL);
    if (calcF == nullptr) {
        PyErr_Print();
    }
    // Get result
    double breathingrateFiltered = PyFloat_AsDouble(calcF);

    if (_visableBasic)
    {
        //    qDebug() << "Base bpm: " << bpmBase;
        ui->lcdBpmB->display(QString::number(bpmBasic));
        //    qDebug() << "Base ibi: " << ibiBase;
        ui->lcdIbiB->display(QString::number(ibiBasic));
        //    qDebug() << "Base sdnn: " << sdnnBase;
        ui->lcdSdnnB->display(QString::number(sdnnBasic));
        //    qDebug() << "Base sdsd: " << sdsdBase;
        ui->lcdSdsdB->display(QString::number(sdsdBasic));
        //    qDebug() << "Base rmssd: " << rmssdBase;
        ui->lcdRmssdB->display(QString::number(rmssdBasic));
        //    qDebug() << "Base hr_mad: " << hrMadBase;
        ui->lcdHrMadB->display(QString::number(hrMadBasic));
        //    qDebug() << "Base breathingrate: " << breathingrateBase;
        ui->lcdBreathB->display(QString::number(breathingrateBasic));
    }
    else
    {
        clearBasic();
    }

    if (_visableFiltered)
    {
        //    qDebug() << "Filtered bpm: " << bpmFiltered;
        ui->lcdBpmF->display(QString::number(bpmFiltered));
        //    qDebug() << "Filtered ibi: " << ibiFiltered;
        ui->lcdIbiF->display(QString::number(ibiFiltered));
        //    qDebug() << "Filtered sdnn: " << sdnnFiltered;
        ui->lcdSdnnF->display(QString::number(sdnnFiltered));
        //    qDebug() << "Filtered sdsd: " << sdsdFiltered;
        ui->lcdSdsdF->display(QString::number(sdsdFiltered));
        //    qDebug() << "Filtered rmssd: " << rmssdFiltered;
        ui->lcdRmssdF->display(QString::number(rmssdFiltered));
        //    qDebug() << "Filtered hr_mad: " << hrMadFiltered;
        ui->lcdHrMadF->display(QString::number(hrMadFiltered));
        //    qDebug() << "Filtered breathingrate: " << breathingrateFiltered;
        ui->lcdBreathF->display(QString::number(breathingrateFiltered));
    }
    else
    {
        clearFiltered();
    }

    Py_DECREF(moduleB);
    Py_DECREF(pythonClassB);
    Py_DECREF(argsF);
    Py_DECREF(moduleF);
    Py_DECREF(pythonClassF);
    Py_DECREF(argsB);
    Py_DECREF(calcB);
    Py_DECREF(methodB);
    Py_DECREF(objectB);
    Py_DECREF(calcF);
    Py_DECREF(methodF);
    Py_DECREF(objectF);
}

void HeartAnalysisPanel::onButtonAnalyzeState(bool state)
{
    qDebug() << "State of check box is: " << state;
}

void HeartAnalysisPanel::onButtonAnalyze(bool state)
{
    emit buttonAnalyzePressed();

    makeAnalysis();
}

void HeartAnalysisPanel::analysisVisableChange(int index, bool visable)
{
    if (index % 2 == 0)
    {
        _visableBasic = visable;
        if (!visable)
            clearBasic();
    }
    else
    {
        _visableFiltered = visable;
        if (!visable)
            clearFiltered();
    }
}

void HeartAnalysisPanel::bufferSampleFull(double* buffer, unsigned size)
{
    emit buttonAnalyzePressed();

    makeAnalysis();
}

void HeartAnalysisPanel::clearBasic()
{
    ui->lcdBpmB->display(QString::number(0));
    ui->lcdIbiB->display(QString::number(0));
    ui->lcdSdnnB->display(QString::number(0));
    ui->lcdSdsdB->display(QString::number(0));
    ui->lcdRmssdB->display(QString::number(0));
    ui->lcdHrMadB->display(QString::number(0));
    ui->lcdBreathB->display(QString::number(0));
}

void HeartAnalysisPanel::clearFiltered()
{
    ui->lcdBpmF->display(QString::number(0));
    ui->lcdIbiF->display(QString::number(0));
    ui->lcdSdnnF->display(QString::number(0));
    ui->lcdSdsdF->display(QString::number(0));
    ui->lcdRmssdF->display(QString::number(0));
    ui->lcdHrMadF->display(QString::number(0));
    ui->lcdBreathF->display(QString::number(0));
}

void HeartAnalysisPanel::onButtonClose(bool state)
{
    clearBasic();
    clearFiltered();
}

void HeartAnalysisPanel::loadVisability(QSettings* settings)
{
    settings->beginGroup(SettingGroup_Channels);
    unsigned size = settings->beginReadArray(SG_Channels_Channel);

    for (unsigned ci = 0; ci < size; ci++)
    {
        settings->setArrayIndex(ci);
        if (ci % 2 == 0)
        {
            _visableBasic = settings->value(SG_Channels_Visible, _visableBasic).toBool();
        }
        else
        {
            _visableFiltered = settings->value(SG_Channels_Visible, _visableFiltered).toBool();
        }
    }

    settings->endArray();
    settings->endGroup();
}
