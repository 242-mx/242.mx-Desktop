/**
 * 242.mx.cpp
 * Implementa el sistema de informaci�n 242.mx elaborado en lenguage C++ con el prorama de desarrollo
 * Microsoft Visual Studio Community 2019 como una aplicaci�n para Desktop nativa de Microsoft Windows
 * con las librer�as de Microsoft Foundations Classes (MFC).
 * @author		Mario Rauz Alejandro Ju�rez Guti�rrez <hola@242.mx>
 * @since		1.0.0
 * @license     GNU GPL version 3 (gnu-gplv3)
 * @copyright	Copyright 2020, Mario Rauz Alejandro Ju�rez Guti�rrez
 * @compile		with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <winsock.h>
#include <wininet.h>
#include <shellapi.h>
#include <strsafe.h>
#include <mmsystem.h>
#pragma comment (lib, "wininet.lib")
#pragma comment(lib, "User32.lib")

 /**
  * EL nombre de la clase de la ventana principal.
  */
static TCHAR className[] = _T("242.mx");

/**
 * La cadena de caracteres que aparece en la barra de t�tulo de la ventana de la aplicaci�n.
 */
static TCHAR windowTitle[] = _T("242.mx");

/**
 * La instancia del sistema.
 */
HINSTANCE hInst;

/**
 * Llamada de la funci�n principal que verifica el directorio local de Google Drive.
 */
int CheckGoogleDrive(TCHAR*);

/**
 * Llamada de la funci�n que descarga el archivo de un isologotipo de Internet.
 */
int DownloadIsologotype(TCHAR*);

/**
 * Llamada la funci�n principal del sistema operativo Windows.
 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * Crea la ventana principal.
 */
int CALLBACK WinMain(
	_In_ HINSTANCE instanceRef,					// Esta instancia de la aplicaci�n
	_In_ HINSTANCE hPrevInstance,				// La instancia previa de la aplicaci�n
	_In_ LPSTR     lpCmdLine,					// El contenido de la l�nea de comandos
	_In_ int       nCmdShow						// �Imprimir los resultados?
)
{
	WNDCLASSEX wcex;							// Estructura de datos de Windows
	TCHAR* buffer;								// Buffer de texto

	// Establece las propiedades de la ventana principal
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instanceRef;
	wcex.hIcon = LoadIcon(instanceRef, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("�Imposible ejecutar la funci�n RegisterClassEx!"),
			_T("242.mx"),
			NULL);

		return 1;
	}

	// Guarda una referencia  al manejador de esta instancia del sistema en una variable global
	hInst = instanceRef;

	// Crea la ventana principal del sistema
	HWND hWnd = CreateWindow(
		className,							// Nombre de la clase
		windowTitle,						// Texto de la barra del t�tulo
		WS_OVERLAPPEDWINDOW,				// Tipo de ventana
		0, 0,								// Posici�n X, Y
		800, 600,							// Ancho y Alto
		NULL,								// Ventana padre
		NULL,								// Sin barra de men�s
		instanceRef,						// Esta instancia del sistema
		NULL								// No utilizado
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("�Imposible crear la ventana principal!"),
			_T("242.mx"),
			NULL);
		return 1;
	}

	// Muestra y actualiza la ventana principal del sisyema
	ShowWindow(hWnd,						// El valor devuelto por el procedimiento CreateWindow
		nCmdShow);							// El cuarto par�metro de la llamada a WinMain
	UpdateWindow(hWnd);

	// Verifica todas las carpetas y archivos locales
	if (buffer = (TCHAR*)malloc(sizeof(TCHAR) * 1024))
	{
		swprintf(buffer, 1024, _T("C:\\Users\\mrajuarez\\Desktop\\"));
		CheckGoogleDrive(buffer);
		free(buffer);
	}
	else
	{
		MessageBox(NULL,
			_T("�Imposible alojar memoria din�micamente para una variable!"),
			_T("242.mx - FATAL ERROR"),
			NULL);
		return 0;
	}
	return 0;
}

/**
 * Verifica la estructura del directorio local de Google Drive.
 */
int CheckGoogleDrive(
	TCHAR *path								// La ruta del folder para verificar
)
{
	FILE *fileDescriptor;					// Descriptor de archivos
	TCHAR *buffer;							// La ruta de un elemento
	WIN32_FIND_DATA finder;					// Buscador de archivos y directorios
	LARGE_INTEGER filesize;					// Tama�o de los archivos
	HANDLE handler = INVALID_HANDLE_VALUE;  // Manejador del buscador de archivos y directorios
	
	// Busca los elementos del directorio local de Google Drive.
	handler = FindFirstFile(path, &finder);
	if (INVALID_HANDLE_VALUE == handler)
	{
		MessageBox(NULL,
			_T("�Imposible verificar el contenido de un directorio de Google Drive!"),
			_T("242.mx - FATAL ERROR"),
			NULL);
		return 1;
	}
	else
	{
		if (buffer = (TCHAR*)malloc(sizeof(TCHAR) * 2048))
		{
			swprintf(buffer, 2048, _T("%s\\desktop.ini"), path);
			if (_wfopen_s(&fileDescriptor, buffer, _T("w")) == 0)
			{
				fwprintf(fileDescriptor,
					_T("[.ShellClassInfo]" \
						"ConfirmFileOp=1" \
						"DefaultDropEffect=4" \
						"NoSharing=1" \
						"Logo=C:\242.mx\Isologotypes\Home.png" \
						"WideLogo=C:\242.mx\Isologotypes\Home.png" \
						"Icon=C:\242.mx\Isologotypes\Home.ico" \
						"IconFile=C:\242.mx\Isologotypes\Home.ico" \
						"IconIndex=0" \
						"IconResource=C:\242.mx\Isologotypes\Home.ico,0" \
						"LocalizedResourceName=BusinessBOK" \
						"InfoTip=Carpeta principal del BusinessBOK que se sincroniza con Google Drive" \
						"[.ExtShellFolderViews]" \
						"{8BEBB290-52D0-11d0-B7F4-00C04FD706EC}={8BEBB290-52D0-11d0-B7F4-00C04FD706EC}" \
						"[ViewState]" \
						"Mode=1" \
						"Vid={0057D0E0-3573-11CF-AE69-08002B2E1262}" \
						"FolderType=Documents"));
				fclose(fileDescriptor);
			}
		}
		else
		{
			MessageBox(NULL,
				_T("�Imposible alojar memoria din�micamente para una variable!"),
				_T("242.mx - FATAL ERROR"),
				NULL);
			return 0;
		}
		CheckGoogleDrive(buffer);
		free(buffer);
		do
		{
			// Si el elemento encontrado es una directorio personalizalo y verifica su contenido
			if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (buffer = (TCHAR*)malloc(sizeof(TCHAR) * 2048))
				{
					swprintf(buffer, 2048, _T("%s\\%s\\desktop.ini"), path, finder.cFileName);
					if (_wfopen_s(&fileDescriptor, buffer, _T("w")) == 0)
					{
						switch(finder.cFileName)
						{
							case "Management", "Administraci�n":
								fwprintf(fileDescriptor,
									_T("[.ShellClassInfo]\n" \
										"CLSID2 = \n" \
										"ConfirmFileOp = 1\n" \
										"NoSharing = 1\n" \
										"Logo = C:\242.mx\Isologotypes\Management.png\n" \
										"IconFile = C:\242.mx\Isologotypes\Management.ico\n" \
										"IconResource = C:\242.mx\Isologotypes\Management.ico, 0" \
										"IconIndex = 0\n" \
										"LocalizedResourceName=Administraci�n\n" \
										"InfoTip = Proceso de Administraci�n del BusinessBOK"));
								break;
							case "AutoCAD":
								break;
							case "Balanced Scorecard":
								break;
							case "Customers":
								break;
							case "Accounting", "Contabilidad":
								break;
							case "Compliance", "Cumplimiento":
								break;
							case "Management System", "Gesti�n del Sistema":
								break;
							case "GitHub.com":
								break;
							case "Real State", "Inmobiliaria":
								break;
							case "Marketing", "Mercadotecnia":
								break;
							case "NeoData":
								break;
							case "Staff", "Personal":
								break;
							case "Drawings", "Planos":
								break;
							case "Projects", "Proyectos":
								break;
							case "Services", "Servicios":
								break;
							case "Sketchup":
								break;
							case "Biblioteca de calibre":
								break;
							case "Pictures", "Im�genes":
								break;
							case "Saved Games", "Juegos guardados":
								break;
							case "Music", "M�sica":
								break;
							case "Software", "Software":
								break;
							case "Videos":
								break;
							case "VirtualBox VMs":
								break;
							case default:
								break;
						}
						fclose(fileDescriptor);
					}
					CheckGoogleDrive(buffer);
					free(buffer);
				}
				else
				{
					MessageBox(NULL,
						_T("�Imposible alojar memoria din�micamente para una variable!"),
						_T("242.mx - FATAL ERROR"),
						NULL);
					return 0;
				}
			}
			// Si el elemento encontrado es un archivo
			else
			{
				filesize.LowPart = finder.nFileSizeLow;
				filesize.HighPart = finder.nFileSizeHigh;
				//_tprintf(TEXT("  %s   %ld bytes\n"), finder.cFileName, filesize.QuadPart);
			}
		} while (FindNextFile(handler, &finder) != 0);
	}
	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		MessageBox(NULL,
			_T("�Imposible verificar el contenido de un directorio de Google Drive!"),
			_T("242.mx - FATAL ERROR"),
			NULL);
		return 0;
	}
	FindClose(handler);

	// Despacha los mensajes recibidos del sistema operativo Windows
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

/**
 * Descarga el archivo de un isologotipo de Internet.
 */
int DownloadIsologotype(TCHAR* url, TCHAR* filename)
{
	HINTERNET connection;					// Conexi�n al servidor
	HINTERNET address;						// Direcci�n IP
	FILE* fileDescriptor;					// Descriptor de archivos
	TCHAR *buffer;							// Buffer de texto
	DWORD i = 0;							// Index

	// Open a connection
	if (!(connection = InternetOpen(_T("Default_User_Agent"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0))) {
		MessageBox(NULL,
			_T("�Imposible conectarse a un servidor en Internet!"),
			_T("242.mx - FATAL ERROR"),
			NULL);
		return 0;
	}
	if (!(address = InternetOpenUrl(connection, url, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION, 0))) {
		MessageBox(NULL,
			_T("�Imposible conectarse a un servidor en Internet!"),
			_T("242.mx - FATAL ERROR"),
			NULL);
		return 0;
	}
	if (buffer = (TCHAR*)malloc(sizeof(TCHAR) * 1024))
	{
		if (_wfopen_s(&fileDescriptor, filename, _T("w")) == 0)
		{
			while (InternetReadFile(address, (char*)buffer, 1024, &i) && (i)) {
				fwrite(buffer, 1, sizeof(buffer), fileDescriptor);
			}
			fclose(fileDescriptor);
		}
		free(buffer);
	}
	
	InternetCloseHandle(address);
	InternetCloseHandle(connection);
	return 0;
}

/**
 * Procesa los mensajes de la ventana principal.
 */
LRESULT CALLBACK WndProc(
	HWND hWnd,								// El valor devuelto por CreateWindow
	UINT message,							// El mensajedle sistema
	WPARAM wParam,							// La variable wParam
	LPARAM lParam							// La variable lParam
)
{
	TCHAR *buffer;							// Buffer de texto
	PAINTSTRUCT ps;							// Pantalla del sistema
	HDC hdc;								// Manejador de la pantalla

	switch (message)
	{
	case WM_PAINT:							// Refresca la ventana principal del sistema
		hdc = BeginPaint(hWnd, &ps);
		if (buffer = (TCHAR*)malloc(sizeof(TCHAR) * 128))
		{
			swprintf(buffer, 128, _T("242.mx"));
			TextOut(hdc, 5, 5, buffer, _tcslen(buffer));
			EndPaint(hWnd, &ps);
			free(buffer);
		}
		else
		{
			// Imprime un mensaje de error
			MessageBox(NULL,
				_T("�Imposible alojar memoria din�micamente para una variable!"),
				_T("242.mx - FATAL ERROR"),
				NULL);
			return 0;
		}
		break;
	case WM_DESTROY:						// Cierra la ventana principal del sistema
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}