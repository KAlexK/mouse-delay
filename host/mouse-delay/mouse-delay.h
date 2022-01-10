#pragma once
#include "stdafx.h"

#define PROG_NAME "Mouse Delay"

#define DEV_ID 60   // ID устройства
//http://wiki.openmoko.org/wiki/USB_Product_IDs#USB_Vendor_and_Product_IDs
//Vendor ID = 0x1d50, Product ID = 0x6043 (pc-power - PC Power Consumption Registrator)
//http://www.ixbt.com/power/device/pc-power.shtml
#define MY_VID 0x1d50 // Vendor ID: 0x1d50 is Openmoko, Inc. Vendor ID
#define MY_PID 0x6043 // Product ID: 0x6043 is PC Power Consumption Registrator

#define CONNECTED 1

#define TOGGLE_LED 0x80   // Toggle LED(s) / Включить-выключить светодиод состояния
#define SW_STATE   0x81 // Передаем значение таймера - задержка
#define SW_ON      0x82 // Включаем кнопку мыши - запускаем измерение задержки
#define UPDATE_FW  0x85    // Перезагрузка в режим обновления микропрограммы
#define GET_DEV_ID 0xA0   // Передача ID устройства

#define DEF_USB_BUFFER_SIZE 64 // Размер буфера для записи и чтения по USB
#define DEF_USB_BUFFER_SIZE_STR "64" // Размер буфера для записи и чтения по USB
#define DEF_DELAY 500 // Задержка между тестами по умолчанию, мс
#define DEF_TESTNUM 100 // Чисто тестов цикле по умолчанию

#define TIMER_ID      101

wxString display_buffer_hex(unsigned char *buffer, unsigned size);

class MyApp: public wxApp
{
public:
  virtual bool OnInit();
};

class MyDlg: public wxDialog
{
public:
  MyDlg();
  ~MyDlg();
private:
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnClose(wxCloseEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnClear(wxCommandEvent& event);
  void OnConnect(wxCommandEvent& event);
  void OnToggle(wxCommandEvent& event);
  void OnUpdateFW(wxCommandEvent& event);
  void OnSendCommand(wxCommandEvent& event);
  void OnSendString(wxCommandEvent& event);
  void OnStart(wxCommandEvent& event);
  bool GetID(unsigned char* id);
  void OnTimer(wxTimerEvent& event);
  void OnLeftDown(wxMouseEvent& event);
  bool USB_write();
  bool USB_read();
  void CloseDev();
  void USBDevError();
  wxSpinCtrl* spinctrl_delay;
  wxSpinCtrl* spinctrl_ntest;
  wxButton* btn_connect;
  wxCheckBox* m_1st_delay;
  wxTextCtrl* ed_command;
  wxTextCtrl* ed_string;
  wxTextCtrl* ed_log;
  wxTimer m_timer;
  bool IsStart; // Запущен замер задержки мыши
  bool IsFirstDelay; // Замер первой задержки (от нажатия кнопки до замыкания контактов)
  int transferred;
  int total_n_test;
  int cur_n_test;
  int test_delay; // Задержка между тестами, мс
  double* delays;
  double* delays1;
  libusb_error error_code;
  libusb_device_handle *MyLibusbDeviceHandle; /* the device handle */
  unsigned char* OutputPacketBuffer; //Allocate a memory buffer which will contain data to send to the USB device
  unsigned char* InputPacketBuffer;  //Allocate a memory buffer for the data which we will read from the USB device
  wxDECLARE_EVENT_TABLE();
};

enum
{
  ID_Hello = 1,
  ID_Connect = 2,
  ID_Toggle = 3
};

