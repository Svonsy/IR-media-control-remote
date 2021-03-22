// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <Windows.h>
#include <stdio.h>
#include <string.h>



int main(void)
{
    HANDLE Port;
    BOOL Status;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char SerialBuffer[64] = { 0 };
    char SerialExampleUP[13] = {"Hello world!"};
    char SerialExampleDOWN[4] = {"Hi!"};
    DWORD dwEventMask;
    unsigned char loop = 0;
    char ReadData;
    DWORD NoBytesRead;
    bool Esc = FALSE;
    bool UP = FALSE;
    bool DOWN = FALSE;

    Port = CreateFile(L"\\\\.\\COM3", GENERIC_READ, 0, NULL, 
    OPEN_EXISTING, 0, NULL);

    if (Port == INVALID_HANDLE_VALUE)
    {
        printf("\nError to Get the COM state\n");
        CloseHandle(Port);
    }
    else
    {
   
        printf("\nopening serial port is succesful\n");

    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(Port, &dcbSerialParams);
    if (Status == FALSE)
    {
        printf("\n Error to Get the COM state \n");
        CloseHandle(Port);
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    Status = SetCommState(Port, &dcbSerialParams);
    if (Status == FALSE)
    {
        printf("\n Error to Setting DCB Structure \n ");
        CloseHandle(Port);
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(Port, &timeouts) == FALSE)
    {
        printf("\n Error to Setting Timeouts");
        CloseHandle(Port);
    }




        while (Esc == FALSE)
        {
            Status = SetCommMask(Port, EV_RXCHAR);

            if (Status == FALSE)
            {
                printf("\nError to in Setting CommMask\n");
                CloseHandle(Port);
            }

            Status = WaitCommEvent(Port, &dwEventMask, NULL);
            if (Status == FALSE)
            {
                printf("\nError! in Setting WaitCommEvent () \n");
                CloseHandle(Port);
            }

            do
            {
                Status = ReadFile(Port, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
                SerialBuffer[loop] = ReadData;
                ++loop;
            } while (NoBytesRead > 0);

            --loop;

            printf("\nNumber of bytes received = % d\n\n", loop);

            for (int i = 0; i <= loop; ++i)
            {
                if (SerialBuffer[i] == SerialExampleUP[i])
                {
                    printf("%c", SerialBuffer[i]);
                    UP = TRUE;
                    DOWN = FALSE;
                }

                else if (SerialBuffer[i] == SerialExampleDOWN[i])
                {
                    printf("%c", SerialBuffer[i]);
                    DOWN = TRUE;
                    UP = FALSE;
                }
            }

            if (UP == TRUE)
            {
                INPUT Input = { 0 };
                Input.type = INPUT_KEYBOARD;
                Input.ki.wVk = VK_VOLUME_UP;
                SendInput(1, &Input, sizeof(Input));
                ZeroMemory(&Input, sizeof(Input));
            }

            else if (DOWN == TRUE)
            {
                INPUT Input = { 0 };
                Input.type = INPUT_KEYBOARD;
                Input.ki.wVk = VK_VOLUME_DOWN;
                SendInput(1, &Input, sizeof(Input));
                ZeroMemory(&Input, sizeof(Input));
            }

            else
            {
                printf("\n Error\n");
            }

            PurgeComm(Port, PURGE_RXCLEAR);
            loop = 0;
        }


    CloseHandle(Port);

}

/*
void Input_Key()
{
    if (SerialBuffer[5] == 'a')
    {
        printf("\n%c\n", SerialBuffer[0]);
                INPUT Input = { 0 };
                Input.type = INPUT_KEYBOARD;
                Input.ki.wVk = VK_VOLUME_UP;
                SendInput(1, &Input, sizeof(Input));
                ZeroMemory(&Input, sizeof(Input));
    }
    else if (SerialBuffer[5] == 'd')
    {
        printf("\n%c\n", SerialBuffer[0]);
        INPUT Input = { 0 };
        Input.type = INPUT_KEYBOARD;
        Input.ki.wVk = VK_VOLUME_DOWN;
        SendInput(1, &Input, sizeof(Input));
        ZeroMemory(&Input, sizeof(Input));
    }

    if (GetAsyncKeyState(VK_ESCAPE)) { Esc = TRUE; }







    while (true)
    {
        Sleep(100);
        if (GetAsyncKeyState(0x41))
        {


        }
        if (GetAsyncKeyState(0x44))
        {
           
        }

    }



}*/


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
