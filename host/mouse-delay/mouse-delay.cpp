// wxwin-test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "mouse-delay.h"

wxBEGIN_EVENT_TABLE(MyDlg, wxDialog)
  EVT_BUTTON(XRCID("Hello"), MyDlg::OnHello)
  EVT_BUTTON(XRCID("EXIT"), MyDlg::OnExit)
  EVT_BUTTON(XRCID("ABOUT"), MyDlg::OnAbout)
  EVT_BUTTON(XRCID("Connect"), MyDlg::OnConnect)
  EVT_BUTTON(XRCID("Toggle"), MyDlg::OnToggle)
  EVT_BUTTON(XRCID("UpdateFW"), MyDlg::OnUpdateFW)
  EVT_BUTTON(XRCID("BtnClear"), MyDlg::OnClear)
  EVT_BUTTON(XRCID("SendCommand"), MyDlg::OnSendCommand)
  EVT_BUTTON(XRCID("SendString"), MyDlg::OnSendString)
  EVT_BUTTON(XRCID("Start!"), MyDlg::OnStart)
  EVT_TIMER(TIMER_ID, MyDlg::OnTimer)
  EVT_LEFT_DOWN(MyDlg::OnLeftDown)
  EVT_LEFT_DCLICK(MyDlg::OnLeftDown)
  EVT_CLOSE(MyDlg::OnClose)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  wxXmlResource::Get()->InitAllHandlers();
  if (!wxXmlResource::Get()->Load("mouse-delay.xrc"))
  {
    wxMessageBox( "Can't load mouse-delay.xrc!", PROG_NAME, wxOK | wxICON_ERROR);
    return false;
  }
  MyDlg *dlg = new MyDlg();
  dlg->Show();
  SetTopWindow(dlg);
  libusb_error error_code = libusb_error(libusb_init(NULL)); /* initialize the library */
  if (error_code != LIBUSB_SUCCESS)
  {
    wxMessageBox(wxString("Can't init libusb:\n") +  libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
    return false;
  }
  return true;
}

MyDlg::MyDlg() : wxDialog(), m_timer(this, TIMER_ID)
{
  if (!wxXmlResource::Get()->LoadDialog(this, 0, "dlg1"))
  {
    wxMessageBox( "Can't load dlg1 from mouse-delay.xrc!", PROG_NAME, wxOK | wxICON_ERROR);
    return;
  }
  spinctrl_delay = XRCCTRL(*this, "spin_ctrl_delay", wxSpinCtrl);
  spinctrl_delay->SetRange(0, 0xffff);
  spinctrl_delay->SetValue(DEF_DELAY);
  spinctrl_ntest = XRCCTRL(*this, "spin_ctrl_ntest", wxSpinCtrl);
  spinctrl_ntest->SetRange(1, 0xffff);
  spinctrl_ntest->SetValue(DEF_TESTNUM);
  btn_connect = XRCCTRL(*this, "Connect", wxButton);
  ed_command = XRCCTRL(*this, "EdCommand", wxTextCtrl);
  ed_string = XRCCTRL(*this, "EdString", wxTextCtrl);
  ed_log = XRCCTRL(*this, "EdLog", wxTextCtrl);
  m_1st_delay = XRCCTRL(*this, "m_1st_delay", wxCheckBox);
  MyLibusbDeviceHandle = 0; /* the device handle */
  IsStart = false; // Не запущен замер задержки мыши
  OutputPacketBuffer = new unsigned char[DEF_USB_BUFFER_SIZE];
  InputPacketBuffer = new unsigned char[DEF_USB_BUFFER_SIZE];
  delays = 0;
  delays1 = 0;
  SetIcon(wxIcon("mouse-delay.ico", wxBITMAP_TYPE_ICO));
}

void MyDlg::USBDevError()
{
  CloseDev();
  btn_connect->SetLabel("Connect");
  m_timer.Stop(); // Останавливаем таймер
  IsStart = false;
}

void MyDlg::CloseDev()
{
  if (MyLibusbDeviceHandle)
  {
    libusb_release_interface(MyLibusbDeviceHandle, 0); //release a previously claimed interface
    libusb_close(MyLibusbDeviceHandle);	//close a device opened
  }
  MyLibusbDeviceHandle = 0;
}


MyDlg::~MyDlg()
{
  delete OutputPacketBuffer;
  delete InputPacketBuffer;
  CloseDev();
  libusb_exit(NULL);
}

void MyDlg::OnExit(wxCommandEvent& event)
{
  Destroy();
}

void MyDlg::OnClose(wxCloseEvent& event)
{
  Destroy(); // make sure you clean up other stuff first.
}

void MyDlg::OnAbout(wxCommandEvent& event)
{
  wxVersionInfo ver =	wxGetLibraryVersionInfo();

  wxMessageBox(wxString("This is a wxWidgets' Hello world sample\n") + ver.ToString(),
    "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyDlg::OnHello(wxCommandEvent& event)
{
  wxLogMessage("Hello world from wxWidgets!");
}

void MyDlg::OnClear(wxCommandEvent& event)
{
  ed_log->Clear();
}

void MyDlg::OnConnect(wxCommandEvent& event)
{
  /*
  Before we can "connect" our application to our USB embedded device, we must first find the device.
  A USB bus can have many devices simultaneously connected, so somehow we have to find our device, and only
  our device.  This is done with the Vendor ID (VID) and Product ID (PID).  Each USB product line should have
  a unique combination of VID and PID. */
  if (!MyLibusbDeviceHandle)  // if not connected already
  {
    wxString diag_msg;
    libusb_device **devs;
    ssize_t cnt;
    cnt = libusb_get_device_list(NULL, &devs); /* find all connected devices */
    if (cnt < 0)
    {
      wxMessageBox(wxString("Can't get device list:\n") + libusb_strerror(libusb_error(cnt)), PROG_NAME, wxOK | wxICON_ERROR);
      return;
    }
    libusb_device *dev;
    libusb_error error_code;
    for (int i = 0; i < cnt; i++)
    {
      if (i)
      {
        diag_msg += ("\n");
      }
      dev = devs[i];
      if (dev == 0)
      {
        break;
      }
      struct libusb_device_descriptor desc;
      error_code = libusb_error(libusb_get_device_descriptor(dev, &desc));
      if (error_code != LIBUSB_SUCCESS)
      {
        wxMessageBox(wxString("Can't get device descriptor:\n") + libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
        //        break;
        continue;
      }
      diag_msg += (wxString::Format(wxT("Try No.%i VID=%#04x PID=%#04x"), i, desc.idVendor, desc.idProduct));
      if (desc.idVendor == MY_VID && desc.idProduct == MY_PID)
      {
        error_code = libusb_error(libusb_open(dev, &MyLibusbDeviceHandle)); //Opens a USB device
        if (error_code != LIBUSB_SUCCESS)
        {
          wxMessageBox(wxString("Can't open device:\n") + libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
          continue;
        }
        error_code = libusb_error(libusb_set_configuration(MyLibusbDeviceHandle, 1)); // Sets the Active configuration of the device
        if (error_code != LIBUSB_SUCCESS)
        {
          wxMessageBox(wxString("Can't set configuration of USB device:\n") + libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
          libusb_close(MyLibusbDeviceHandle);  //Closes a device opened since the set configuration is failed.
          MyLibusbDeviceHandle = 0;
          continue;
        }
        else
        {
          error_code = libusb_error(libusb_claim_interface(MyLibusbDeviceHandle, 0)); //claims the interface with the Operating System
          if (error_code != LIBUSB_SUCCESS)
          {
            wxMessageBox(wxString("Can't claim interface of USB device:\n") + libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
            libusb_close(MyLibusbDeviceHandle);  //Closes a device opened since the claim interface is failed.
            MyLibusbDeviceHandle = 0;
            continue;
          }
        }
        {
          unsigned char cur_id;
          if (GetID(&cur_id))
          {
            diag_msg += (wxString::Format(wxT(" ID=%i"), cur_id));
            if (cur_id == DEV_ID)
            {
              break;
            }
          }
          libusb_release_interface(MyLibusbDeviceHandle, 0); //release a previously claimed interface
          libusb_close(MyLibusbDeviceHandle);	//close a device opened
          MyLibusbDeviceHandle = 0;
        }
      }
    }
    libusb_free_device_list(devs, 1);
    if (!MyLibusbDeviceHandle)
    {
      wxString msg("Can't connect to USB device!");
      wxMessageBox(msg, PROG_NAME, wxOK | wxICON_ERROR);
      return;
    }
    diag_msg += (wxString("\nFound!\n"));
    wxMessageBox(wxString("Connected!\n") + diag_msg, PROG_NAME, wxOK | wxICON_INFORMATION);
    btn_connect->SetLabel("Disconnect");
    //    EnableWindows(true);
  }
  else
  {
    USBDevError();
    wxMessageBox("Disconnected!", PROG_NAME, wxOK | wxICON_INFORMATION);
  }
}

bool MyDlg::USB_write()
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return false;
  }
  else
  {
    error_code = libusb_error(libusb_bulk_transfer(MyLibusbDeviceHandle, 0x01, OutputPacketBuffer, DEF_USB_BUFFER_SIZE, &transferred, 5000));
	  if (error_code != LIBUSB_SUCCESS)
    {
      USBDevError();
      wxMessageBox(wxString("Error in USB bulk write:\n") +  libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
      return false;
    }
    if (transferred != DEF_USB_BUFFER_SIZE)
    {
      USBDevError();
      wxMessageBox("USB bulk written bytes != " DEF_USB_BUFFER_SIZE_STR "!", PROG_NAME, wxOK | wxICON_ERROR);
      return false;
    }
  }
  return true;
}

bool MyDlg::USB_read()
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return false;
  }
  else
  {
    error_code = libusb_error(libusb_bulk_transfer(MyLibusbDeviceHandle, 0x81, InputPacketBuffer, DEF_USB_BUFFER_SIZE, &transferred, 5000));
    if (error_code != LIBUSB_SUCCESS)
    {
      USBDevError();
      wxMessageBox(wxString("Error in USB bulk read:\n") +  libusb_strerror(error_code), PROG_NAME, wxOK | wxICON_ERROR);
      return false;
    }
    if (transferred != DEF_USB_BUFFER_SIZE)
    {
      USBDevError();
      wxMessageBox("USB bulk read bytes != " DEF_USB_BUFFER_SIZE_STR "!", PROG_NAME, wxOK | wxICON_ERROR);
      return false;
    }
  }
  return true;
}

void MyDlg::OnStart(wxCommandEvent& event)
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return;
  }
  SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
  IsFirstDelay = m_1st_delay->IsChecked();
  total_n_test = spinctrl_ntest->GetValue();
  delays = new double[total_n_test];
  if (IsFirstDelay)
  {
    delays1 = new double[total_n_test];
  }
  cur_n_test = 1; // Запускаем первый тест
  test_delay = spinctrl_delay->GetValue();
  m_timer.Start(test_delay, true); // Задержка до следующего теста
}

void MyDlg::OnTimer(wxTimerEvent& event)
{
//  m_timer.Stop(); // Останавливаем таймер
  OutputPacketBuffer[0] = SW_ON; // Включаем кнопку мыши - запускаем измерение задержки
  IsStart = true; // Запущен замер задержки мыши
  USB_write();
}

void MyDlg::OnLeftDown(wxMouseEvent& event)
{
  if (IsStart)// Запущен замер задержки мыши
  {
    OutputPacketBuffer[0] = SW_STATE; // Передача ID устройства
    if (USB_write())
    {
      if (USB_read())
      {
        if (InputPacketBuffer[0] != SW_STATE)
        {
          USBDevError();
          wxMessageBox("InputPacketBuffer[0] != SW_STATE! in OnLeftDown()", PROG_NAME, wxOK | wxICON_ERROR);
          return;
        }
        else
        {
          uint32_t delay_tick = ((uint32_t*)(InputPacketBuffer + 2))[0]; // Сохраняем значение задержки в отсчетах таймера
          double delay_ms = 1000.0 * double(delay_tick) / (48000000.0 / 1024.0);
          delays[cur_n_test - 1] = delay_ms;
          if (IsFirstDelay)
          {
            delay_tick = ((uint32_t*)(InputPacketBuffer + 2 + 4))[0]; // Сохраняем значение первой задержки в отсчетах таймера
            delay_ms = 1000.0 * double(delay_tick) / (48000000.0 / 1024.0);
            delays1[cur_n_test - 1] = delay_ms;
            ed_log->SetValue(wxString::Format(wxT("%i->%.1f (%.2f)\n"), cur_n_test, delays[cur_n_test - 1], delay_ms));
          }
          else
          {
            ed_log->SetValue(wxString::Format(wxT("%i->%.1f\n"), cur_n_test, delay_ms));
          }
          IsStart = false; // Не запущен замер задержки мыши
          cur_n_test++;
          if (cur_n_test > total_n_test) // Завершаем цикл тестов
          {
            ed_log->Clear();
            double delays_sum = delays[0];
            double min_delay = delays[0];
            double max_delay = delays[0];
            for (int i = 1; i < total_n_test; i++)
            {
              delays_sum += delays[i];
              min_delay = min(min_delay, delays[i]);
              max_delay = max(max_delay, delays[i]);
            }
            double aver_delay = delays_sum / ((double)total_n_test);
            if (IsFirstDelay)
            {
              double delays_sum1 = delays1[0];
              double min_delay1 = delays1[0];
              double max_delay1 = delays1[0];
              for (int i = 1; i < total_n_test; i++)
              {
                delays_sum1 += delays1[i];
                min_delay1 = min(min_delay1, delays1[i]);
                max_delay1 = max(max_delay1, delays1[i]);
              }
              double aver_delay1 = delays_sum1 / ((double)total_n_test);
              ed_log->AppendText(wxString::Format(wxT("Average delay\t%.1f\t%.2f\n"), aver_delay, aver_delay1));
              ed_log->AppendText(wxString::Format(wxT("Minimum delay\t%.1f\t%.2f\n"), min_delay, min_delay1));
              ed_log->AppendText(wxString::Format(wxT("Maximum delay\t%.1f\t%.2f\n"), max_delay, max_delay1));
              ed_log->AppendText("List of delays:\n");
              for (int i = 0; i < total_n_test; i++)
              {
                ed_log->AppendText(wxString::Format(wxT("%.1f\t%.2f\n"), delays[i], delays1[i]));
              }
            }
            else
            {
              ed_log->AppendText(wxString::Format(wxT("Average delay\t%.1f\n"), aver_delay));
              ed_log->AppendText(wxString::Format(wxT("Minimum delay\t%.1f\n"), min_delay));
              ed_log->AppendText(wxString::Format(wxT("Maximum delay\t%.1f\n"), max_delay));
              ed_log->AppendText("List of delays:\n");
              for (int i = 0; i < total_n_test; i++)
              {
                ed_log->AppendText(wxString::Format(wxT("%.1f\n"), delays[i]));
              }
            }
            delete delays; 
            delays = 0;
            delete delays1;
            delays1 = 0;
            SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
          }
          else // Иначе продолжаем измерения
          {
            m_timer.Start(test_delay, true); // Задержка до следующего теста
          }
        }
      }
    }
  }
}


void MyDlg::OnToggle(wxCommandEvent& event)
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return;
  }
  OutputPacketBuffer[0] = TOGGLE_LED;		   //TOGGLE_LED (0x80) is the "Toggle LED(s)" command in the firmware
                         //For simplicity, we will leave the rest of the buffer uninitialized, but you could put real
                         //data in it if you like.
  //Writes data to a bulk endpoint. The Function call will send out ep_size bytes to the USB Device.
  USB_write();
}

void MyDlg::OnUpdateFW(wxCommandEvent& event)
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return;
  }
  OutputPacketBuffer[0] = UPDATE_FW;		   // Перезагрузка в режим обновления микропрограммы
  USB_write();
}

void MyDlg::OnSendCommand(wxCommandEvent& event) // Write+Read
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return;
  }
  wxString str_val = ed_command->GetValue();
  wxStringTokenizer tokenizer(str_val, " ");
  int i = 0;
  while (tokenizer.HasMoreTokens())
  {
    wxString token = tokenizer.GetNextToken();
    unsigned long val;
    if (token.ToULong(&val, 16))
    {
      OutputPacketBuffer[i] = (unsigned char)val;
      i++;
    }
    else
    {
      break;
    }
  }
  int len = i;
  for (; i < sizeof(OutputPacketBuffer); i++)
  {
    OutputPacketBuffer[i] = 0;
  }
  ed_log->AppendText("To write:"); 	
  ed_log->AppendText(display_buffer_hex(OutputPacketBuffer, len)); 	
  if (USB_write())
  { 
    USB_read();
    ed_log->AppendText(display_buffer_hex(InputPacketBuffer, transferred)); 	
  }
}

void MyDlg::OnSendString(wxCommandEvent& event) // Write
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return;
  }
  wxString str_val = ed_string->GetValue();
  wxStringTokenizer tokenizer(str_val, " ");
  int i = 0;
  while (tokenizer.HasMoreTokens())
  {
    wxString token = tokenizer.GetNextToken();
    unsigned long val;
    if (token.ToULong(&val, 16))
    {
      OutputPacketBuffer[i] = (unsigned char)val;
      i++;
    }
    else
    {
      break;
    }
  }
  int len = i;
  for (; i < sizeof(OutputPacketBuffer); i++)
  {
    OutputPacketBuffer[i] = 0;
  }
  ed_log->AppendText("To write:"); 	
  ed_log->AppendText(display_buffer_hex(OutputPacketBuffer, len)); 	
  USB_write();
}

bool MyDlg::GetID(unsigned char* id)
{
  if (!MyLibusbDeviceHandle)
  {
    wxMessageBox("Not connected!", PROG_NAME, wxOK | wxICON_INFORMATION);
    return false;
  }
  OutputPacketBuffer[0] = GET_DEV_ID; // Передача ID устройства
  if (USB_write())
  { 
    if (USB_read())
    {
      if (InputPacketBuffer[0] != GET_DEV_ID)
      {
        wxMessageBox("InputPacketBuffer[0] != GET_DEV_ID! in GetID()", PROG_NAME, wxOK | wxICON_ERROR);
      }
      else
      {
        *id = InputPacketBuffer[1];
        return true;
      }
    }
  }
  return false;
}

wxString display_buffer_hex(unsigned char *buffer, unsigned size)
{
  unsigned i, j, k;
  wxString res;
  for (i = 0; i < size; i += 16)
  {
    //		printf("\n  %08x  ", i);
    res += wxString::Format(wxT("\n  %08x  "), i);
    for (j = 0, k = 0; k < 16; j++, k++)
    {
      if (i + j < size)
      {
        //				printf("%02x", buffer[i+j]);
        res += wxString::Format(wxT("%02x"), buffer[i + j]);
      }
      else
      {
        //				printf("  ");
        res += "  ";
      }
      //			printf(" ");
      res += " ";
    }
    //		printf(" ");
    res += " ";
    for (j = 0, k = 0; k < 16; j++, k++)
    {
      if (i + j < size)
      {
        if ((buffer[i + j] < 32) || (buffer[i + j] > 126))
        {
          //					printf(".");
          res += ".";
        }
        else
        {
          //					printf("%c", buffer[i+j]);
          res += wxString::Format(wxT("%c"), buffer[i + j]);
        }
      }
    }
  }
  //	printf("\n" );
  res += "\n";
  return res;
}

