/*
// PIXXY
// A tool for image measurement.
// (WIN32 application)
//
// Copyright(c) aoyama iwao
*/

// Required to process WM_MOUSEWHEEL massages
// and pass WS_EX_COMPOSITED to CreateWindowsEx function.
#define		_WIN32_WINNT	0x0501

#define  PAGER_WIDTH   2000
#define  PAGER_HEIGHT  30
#define  STATUSBAR_HEIGHT  20

// Toolbar
#define     ID_TOOLBAR   2000
// Statusbar
#define     ID_STATUS    2001

// Common controls
#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include <commctrl.h> 

// Using stb_image by Sean Barrett
// Visit http://github.com/nothings/stb for detail.
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include "pixxy.h"
#include <math.h>

// Forward declarations
// Main window procedure
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
// About dialog procedure
BOOL CALLBACK AboutDlgProc( HWND, UINT, WPARAM, LPARAM) ;
// Processing messages sent to the open dialog
BOOL CALLBACK ExplorerHook(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
// Processing WM_NOTIFY sent to the open dialog
BOOL NEAR PASCAL OnNotify(HWND hDlg, LPOFNOTIFY pOFN);

// Functions for displaying images
int     Open_A_imagefile( HWND, const char* );
int		loadimage_2_offscreen( HWND, const char* );
HBITMAP LoadStbImage( HWND, const char* );
void    On_paint( HDC );
double     set_magPower(int);
int    get_pixel_coord_on_mouse( HWND, int*, int* );
int    round(double);

// Functions for scrolling
void	Scroll_on_redraw(HWND);
void	Horiz_scroll(HWND, WPARAM);
void	Vert_scroll(HWND, WPARAM);
// End forward declarations

// Globals
// Images
int			w = 0, h = 0; // Width and height of your image. Used for displaying and scroling.
int          bpp;         // Bit plane number.
unsigned char* pixels ;   // Pointer to your image.

// Bitmaps
BITMAPINFOHEADER  b = { sizeof(b) };
HBITMAP			hBitmap;

// File handlings
char pstrFileName[MAX_PATH] = {'\0'};  // Filename with full path.
char pstrTitleName[MAX_PATH]= {'\0'};  // Filename without full path.

// Enlarge or shrink image
double       mag = 1.0 ; // magnifying power
enum { lifesize, one_of_8 = -6, one_of_6, one_of_4, third, half, two_of_3, 
                 twice = 1, triple, four_times, five_times, six_times, seven_times };
int mp  = lifesize;

// Main window title
char szWindowTitle[256]; // Main window title
char   szMag[16] = {'\0'}; // Magnifying power on window title bar

// Off-screen
HDC				hdcMem;

// Scrolling
POINT				org			= {0, 0};		// org.x is the current vertical scrollbox position.
												// org.y is the current horizontal scrollbox position.
RECT				rc;							// Size of the client area.			
SCROLLINFO			si			= {0};			// Scrollbar info.
int					iVertPos, iHorzPos;

// Variables relate to the mouse position.
POINT          ptOnScreen;
POINT          ptOnClient;
char szMousePos[256];
char szMouseXPos[128];
char szMouseYPos[128];

// Pager overrided by the toolbar
HWND    hPager;

// Toolbar
HWND hToolBar;

// Buttons on the toolbar
TBBUTTON  tbb[] = 
{
	// Toolbar standard buttons (open image)(close image)
	{STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON },
	{STD_DELETE, IDM_CLOSE, TBSTATE_ENABLED, TBSTYLE_BUTTON },

	{ 0,0, TBSTATE_ENABLED, TBSTYLE_SEP }, // A separator

	// Owner-drawn button bitmaps (Reload image)(Enlarge image)(Shrink image)
	{0, IDM_RELOAD, TBSTATE_ENABLED, TBSTYLE_BUTTON },
	{0, IDM_ENLARGE, TBSTATE_ENABLED, TBSTYLE_BUTTON },
	{0, IDM_SHRINK, TBSTATE_ENABLED, TBSTYLE_BUTTON }
};

// Tooltips on the toolbar
LPTOOLTIPTEXT  lpttt;

// Statusbar
HWND  hStatus;

// Tooltip on the statusbar
HWND   hStatusToolTip;
TOOLINFO  ti;
char  szDsc[128];
// End globals

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("pixxy") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
	 HACCEL       hAccel;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
	 wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON)) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = "PIXXYMAINMENU" ;  
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Windows NT or later is required!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

	 strcpy( szWindowTitle, szAppName );

     hwnd = CreateWindowEx ( WS_EX_COMPOSITED,
		                  szAppName,                  // window class name
						  szWindowTitle,              // window captio (display on titlebar)
                          WS_OVERLAPPEDWINDOW,        // window style
                          CW_USEDEFAULT,              // initial x position (upper-left corner)
                          CW_USEDEFAULT,              // initial y opsition (upper-left corner)
                          CW_USEDEFAULT,              // initial window width
                          CW_USEDEFAULT,              // initial window height
                          NULL,                       // parent window title
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                    
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     hAccel = LoadAccelerators( hInstance, "PIXXYACCEL" );

     while (GetMessage (&msg, NULL, 0, 0))
     {
		 if (!TranslateAccelerator (hwnd, hAccel, &msg))
		 {
             TranslateMessage (&msg) ;
             DispatchMessage (&msg) ;
		 }
     }

     return msg.wParam ;
}

LRESULT CALLBACK WndProc(HWND hImageWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC				hdc;
	PAINTSTRUCT		ps;

	// pop-up menu
	HMENU hPopupMenu;

	// pixel coordinates on the statusbar
	int x, y;
	char str[256];

	// The clipboard
	HGLOBAL    hGlobal;
	char*      pGlobal;

	// Toolbar
	int toolbarID; // The first index of toolbar bitmaps, i.e. return value of SendMesaage( , TB_ADDBITMAP, , ,).

	TBADDBITMAP  tbab; // Bitmaps added to the toolbar

	static TRACKMOUSEEVENT tme;

	switch(msg)
	{
		case WM_CREATE:

			InitCommonControls();

			// Create a pager overrided by the toolbar
			hPager = CreateWindowEx(0, WC_PAGESCROLLER, NULL,
				                       WS_CHILD | WS_VISIBLE | PGS_HORZ,
									   0, 0,
									   PAGER_WIDTH, PAGER_HEIGHT,
									    hImageWnd,
										NULL, HINST_COMMCTRL, NULL );

			// Create a toolbar
			hToolBar = CreateToolbarEx( hPager,
				                        WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS ,
										ID_TOOLBAR,
										1,
                                        HINST_COMMCTRL, 
										IDB_STD_SMALL_COLOR, 
										tbb,
										1,
										0,0,
										0,0,
										sizeof(TBBUTTON) );

			// Make it Explorer-style flat-toolber
			SetWindowLong( hToolBar, GWL_STYLE, GetWindowLong( hToolBar, GWL_STYLE) | TBSTYLE_FLAT );

			// Add a close button (standard toolbar button) to the toolbar
			tbab.hInst = HINST_COMMCTRL;    // standard toolbar
			tbab.nID = IDB_STD_SMALL_COLOR; // small standard toolbar

			toolbarID = SendMessage( hToolBar, TB_ADDBITMAP, // Add a bitmap to the toolbar
										1,                 // Number of a adding image
										(LPARAM)&tbab); 

			tbb[1].iBitmap += toolbarID;

			SendMessage( hToolBar, TB_ADDBUTTONS, // Add buttons to the toolbar
				              2, // Number of a added button plus one (a separator)
							  (LPARAM)&tbb[1] );  // The array argument is the number of buttons before button addition
			// End adding a close button

			// Add a refresh button to the toolbar
			tbab.hInst = ((LPCREATESTRUCT)lp)->hInstance;
			tbab.nID = IDB_RELOAD;

			toolbarID = SendMessage( hToolBar, TB_ADDBITMAP, // Add a bitmap to the toolbar
										1,                 // Number of a adding image
										(LPARAM)&tbab); 

			tbb[3].iBitmap += toolbarID;

			SendMessage( hToolBar, TB_ADDBUTTONS, // Add a button to the toolbar
				              1, // Number of a adding button
							  (LPARAM)&tbb[3] );  // The array argument is the number of buttons before button addition
			// End adding a refresh button

			// Add a enlarge button to the toolbar
			tbab.nID = IDB_ENLARGE;

			toolbarID = SendMessage( hToolBar, TB_ADDBITMAP, // Add a bitmap to the toolbar
										1,                 // Number of a adding bitmap
										(LPARAM)&tbab); 

			tbb[4].iBitmap += toolbarID;

			SendMessage( hToolBar, TB_ADDBUTTONS, // Add a button to the toolbar
				              1, // Number of a adding bitmap
							  (LPARAM)&tbb[4] );  // The array argument is the number of buttons before button addition
			// End add a enlarge button

			// Add a shrink button to the toolbar
			tbab.nID = IDB_SHRINK;

			toolbarID = SendMessage( hToolBar, TB_ADDBITMAP, // Add a bitmap to the toolbar
										1,                 // Number of a adding bitmap
										(LPARAM)&tbab); 

			tbb[5].iBitmap += toolbarID;

			SendMessage( hToolBar, TB_ADDBUTTONS, // Add a button to the toolbar
				              1, // Number of a button
							  (LPARAM)&tbb[5] );  // The array argument is the number of buttons before button addition
			// End adding a shrink toolbar

			// Initial menus and the toolbar status
			EnableMenuItem( GetMenu( hImageWnd ), IDM_OPEN, MF_ENABLED);
			EnableMenuItem( GetMenu( hImageWnd ), IDM_CLOSE, MF_GRAYED);
			EnableMenuItem( GetMenu( hImageWnd ), IDM_RELOAD, MF_GRAYED);
			EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_GRAYED);
			EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_GRAYED);
                      
            SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_OPEN, TRUE);
			SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_CLOSE, FALSE);
			SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_RELOAD, FALSE);
			SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, FALSE);
			SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, FALSE);

			// Statusbar
			hStatus = CreateWindowEx( 0,
			                          STATUSCLASSNAME,
									  NULL,
									  WS_CHILD | SBARS_SIZEGRIP | CCS_BOTTOM | WS_VISIBLE,
									  0, 0,
									  0, STATUSBAR_HEIGHT,
									  hImageWnd,
									  (HMENU)ID_STATUS,
									  ((LPCREATESTRUCT)lp)->hInstance,
									  NULL);
			SendMessage( hStatus, SB_SIMPLE, TRUE, 0L);

			// Tooltip for the Statusbar
			hStatusToolTip = CreateWindowEx( 0,
					                        TOOLTIPS_CLASS,
											NULL,
											TTS_ALWAYSTIP,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											CW_USEDEFAULT,
											hImageWnd,
											NULL,
											((LPCREATESTRUCT)lp)->hInstance,
											NULL);
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_SUBCLASS;
			ti.hwnd = hStatus;
			ti.hinst = ((LPCREATESTRUCT)lp)->hInstance;
			ti.uId = ID_STATUS;
			ti.rect.top = 0;
			ti.rect.left = 0;
			ti.rect.bottom = STATUSBAR_HEIGHT;
			ti.rect.right = 80;
			LoadString( ((LPCREATESTRUCT)lp)->hInstance, IDS_COORD, szDsc, 128 );
			ti.lpszText = (LPTSTR)szDsc;

			SendMessage( hStatusToolTip, TTM_ADDTOOL, 0, (LPARAM)&ti );

			// Initialize TRACKMOUSEEVENT structure
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hImageWnd;

			break;

		case WM_PAINT:
			hdc	= BeginPaint(hImageWnd, &ps);
			On_paint(hdc);
			EndPaint(hImageWnd, &ps);
			break;

		case WM_SIZE:
			// Fit the pager width to the main window width to show the pager when the window shrinks
			// MoveWindow( hPager, 0, 0, LOWORD(lp), PAGER_HEIGHT, TRUE );

			SendMessage( hStatus, WM_SIZE, wp, lp );

			Scroll_on_redraw(hImageWnd);

			hdc = GetDC(hImageWnd);
			On_paint(hdc);

			ReleaseDC(hImageWnd, hdc);
			
			return ( DefWindowProc(hImageWnd, msg, wp, lp) );

		case WM_HSCROLL:
			Horiz_scroll(hImageWnd, wp);

			hdc = GetDC(hImageWnd);
			On_paint(hdc);
			ReleaseDC(hImageWnd, hdc);

			break;

		case WM_VSCROLL:
			Vert_scroll(hImageWnd, wp);

			hdc = GetDC(hImageWnd);
			On_paint(hdc);
			ReleaseDC(hImageWnd, hdc);

			break;

		case WM_MOUSEWHEEL:

			// make disable when the vertical scrollbar is hided
			GetClientRect(hImageWnd, &rc);
			if( (rc.bottom - rc.top) - PAGER_HEIGHT - STATUSBAR_HEIGHT > mag * h  )  break;

			if( (short)HIWORD(wp) > 0 ) 
			{
				SendMessage(hImageWnd, WM_VSCROLL, SB_LINEUP, 0); 
				SendMessage(hImageWnd, WM_VSCROLL, SB_LINEUP, 0);
				break;
			}

			else if( (short)HIWORD(wp) < 0 ) 
			{
				SendMessage(hImageWnd, WM_VSCROLL, SB_LINEDOWN, 0); 
				SendMessage(hImageWnd, WM_VSCROLL, SB_LINEDOWN, 0); 
				break;
			}

		case WM_MOUSEMOVE:
			get_pixel_coord_on_mouse( hImageWnd, &x, &y );
			_TrackMouseEvent(&tme);
			wsprintf( str, "X: %d Y: %d", x, y );
			SendMessage( hStatus, SB_SETTEXT, 255|0, (WPARAM)(LPSTR)str);
			break;

		case WM_MOUSELEAVE:
			x=0;
			y=0;
			wsprintf( str, "X: %d Y: %d", x, y );
			SendMessage( hStatus, SB_SETTEXT, 255|0, (WPARAM)(LPSTR)str);
			break;

		case WM_DESTROY:
			if( hdcMem != NULL ) DeleteDC (hdcMem) ;
			if( pixels != NULL ) stbi_image_free(pixels);
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			switch(LOWORD(wp))
		    {
			  case IDM_END:
				SendMessage(hImageWnd, WM_DESTROY, 0, 0);
				break;

			  case IDM_OPEN:

			      if( Open_A_imagefile( hImageWnd, pstrFileName ) == 0 ) break;
				  
				  Scroll_on_redraw(hImageWnd); // Show scrollbars

				  // Refresh menus and the toolbar status if image is loaded
				  if( pixels != NULL )
				  {
			         EnableMenuItem( GetMenu( hImageWnd ), IDM_OPEN, MF_GRAYED);
			         EnableMenuItem( GetMenu( hImageWnd ), IDM_CLOSE, MF_ENABLED);
		   	         EnableMenuItem( GetMenu( hImageWnd ), IDM_RELOAD, MF_ENABLED);
					 EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_ENABLED);
			         EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_ENABLED);

                     SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_OPEN, FALSE);
			         SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_CLOSE, TRUE);
			         SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_RELOAD, TRUE);
					 SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, TRUE);
			         SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, TRUE);
				   }

				  break;

				case IDM_CLOSE:

				  if( w != 0 || h != 0 ) // Image is already loaded
				  {				  
				       hdc = GetDC( hImageWnd );
                         PatBlt(hdc, 0, 0, 2*mag*w, 2*mag*h, WHITENESS );
                       ReleaseDC( hImageWnd, hdc );

					   stbi_image_free(pixels);
					   pixels = NULL;
					   w = 0;
					   h = 0;
					   org.x = 0;
					   org.y = 0;
					   mag = 1.0;
					   mp = lifesize;

					   // Hide scrollbars
                       si.fMask = SIF_RANGE;
					   si.nMax = 0;
					   SetScrollInfo(hImageWnd, SB_HORZ, &si, TRUE);

					   si.nMax = 0;
					   SetScrollInfo(hImageWnd, SB_VERT, &si, TRUE);

					   // Refresh menus and the toolbar status
					   EnableMenuItem( GetMenu( hImageWnd ), IDM_OPEN, MF_ENABLED);
			           EnableMenuItem( GetMenu( hImageWnd ), IDM_CLOSE, MF_GRAYED);
			           EnableMenuItem( GetMenu( hImageWnd ), IDM_RELOAD, MF_GRAYED);
					   EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_GRAYED);
                       EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_GRAYED);

                       SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_OPEN, TRUE);
			           SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_CLOSE, FALSE);
		               SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_RELOAD, FALSE);
					   SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, FALSE);
		               SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, FALSE);

					   // Refresh the main window title
					   	pstrFileName[0] = '\0';
						pstrTitleName[0]= '\0';
						strcpy( szWindowTitle, "pixxy" );

					    SetWindowText( hImageWnd, szWindowTitle );
				  }

			     break;

			  case IDM_COPY_COORDINATES:

				  // Initialize or flush the previous coordinates
				  szMousePos[0] = '\0';
				  szMouseXPos[0] = '\0';
				  szMouseYPos[0] = '\0';

				  sprintf( szMouseXPos, "%d", round((double)(ptOnClient.x + org.x)/mag) );
				  strcat( szMousePos, szMouseXPos );
				  strcat( szMousePos, "\t" ); 
				  sprintf( szMouseYPos, "%d", round((double)(ptOnClient.y + org.y + 2 - PAGER_HEIGHT)/mag) );// +2 is border width correction
				  strcat( szMousePos, szMouseYPos );

				  if( !szMousePos ) break;

				  hGlobal = GlobalAlloc( GHND | GMEM_SHARE, (lstrlen(szMousePos) +1 ) * sizeof(TCHAR) );
				  pGlobal = (char*)GlobalLock(hGlobal);
				  lstrcpy(pGlobal, szMousePos);
				  GlobalUnlock(hGlobal);

				  OpenClipboard( hImageWnd );
				  EmptyClipboard();
				  SetClipboardData( CF_TEXT, hGlobal );
				  CloseClipboard();

				  break;

			  case IDM_COPY_COORDINATES_ACCEL:

				  // Initialize or flush the previous coordinates
				  szMousePos[0] = '\0';
				  szMouseXPos[0] = '\0';
				  szMouseYPos[0] = '\0';

				  get_pixel_coord_on_mouse( hImageWnd, &x, &y );
				  sprintf( szMouseXPos, "%d", x );
				  strcat( szMousePos, szMouseXPos );
				  strcat( szMousePos, "\t" );
				  sprintf( szMouseYPos, "%d", y );
				  strcat( szMousePos, szMouseYPos );
				  if( !szMousePos ) break;

				  hGlobal = GlobalAlloc( GHND | GMEM_SHARE, (lstrlen(szMousePos) +1 ) * sizeof(TCHAR) );
				  pGlobal = (char*)GlobalLock(hGlobal);
				  lstrcpy(pGlobal, szMousePos);
				  GlobalUnlock(hGlobal);

				  OpenClipboard( hImageWnd );
				  EmptyClipboard();
				  SetClipboardData( CF_TEXT, hGlobal );
				  CloseClipboard();

				  break;

			  case IDM_RELOAD:

				  if( pixels != NULL )
				  {
			          loadimage_2_offscreen( hImageWnd, pstrFileName );
					  InvalidateRect( hImageWnd, NULL, TRUE);
				  }

				  break;

			  case IDM_ENLARGE:

				  mp++;

				  if( mp >= seven_times ) // More than the upper limit
				  {
					  mp = seven_times;

					  EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_GRAYED);
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_ENABLED);
                      SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, FALSE);
					  SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, TRUE);
				  }

				  else
				  {
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_ENABLED);
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_ENABLED);
                      SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, TRUE);
					  SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, TRUE);
				  }

				  set_magPower( mp );

				  // Show the loaded image filename on the titlebar
				  szWindowTitle[0] = '\0';
		          strcpy( szWindowTitle, "pixxy - " );
		          strcat( szWindowTitle, pstrTitleName );


				  // Show the magnifying power on the titlebar
		          strcat( szWindowTitle, " - " );
		          sprintf( szMag, "%3.1f", mag*100 );
		          strcat( szWindowTitle, szMag );
		          strcat( szWindowTitle, " \%" );

				  SetWindowText( hImageWnd, szWindowTitle );

				  InvalidateRect( hImageWnd, NULL, TRUE ); // Refresh image
				  Scroll_on_redraw( hImageWnd ); // Refresh scrollbars status

				  break;

			  case IDM_SHRINK:

				  mp--;

				  if( mp <= one_of_8 ) // Less than the lower limit
				  {
					  mp = one_of_8;

					  EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_ENABLED);
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_GRAYED);
					  SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, TRUE);
                      SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, FALSE);
				  }

				  else
				  {
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_ENLARGE, MF_ENABLED);
					  EnableMenuItem( GetMenu( hImageWnd ), IDM_SHRINK, MF_ENABLED);
					  SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_ENLARGE, TRUE);
                      SendMessage( hToolBar, TB_ENABLEBUTTON, IDM_SHRINK, TRUE);
				  }

				  set_magPower( mp );
				  
				  // Show the loaded image filename on the titlebar
				  szWindowTitle[0] = '\0';
		          strcpy( szWindowTitle, "pixxy - " );
		          strcat( szWindowTitle, pstrTitleName );

		          // Show the magnifying power on the titlebar
		          strcat( szWindowTitle, " - " );
		          sprintf( szMag, "%3.1f", mag*100 );
		          strcat( szWindowTitle, szMag );
		          strcat( szWindowTitle, " \%" );

				  SetWindowText( hImageWnd, szWindowTitle );

				  InvalidateRect( hImageWnd, NULL, TRUE ); // Refresh image
				  Scroll_on_redraw( hImageWnd ); // Refresh scrollbars status

				  break;

			  case IDM_ABOUT:

				  DialogBox( (HINSTANCE)GetWindowLong( hImageWnd, GWL_HINSTANCE), 
					           "ABOUTDIALOG",
					            hImageWnd, AboutDlgProc );
				  break;

			  default:
				break;
		    }

		     break;

		case WM_RBUTTONDOWN:
			GetCursorPos( &ptOnClient );
			ScreenToClient(hImageWnd, &ptOnClient );

			// Show the pop-up menu on the cursor position
			GetCursorPos(&ptOnScreen);

			hPopupMenu = LoadMenu( (HINSTANCE)GetWindowLong(hImageWnd, GWL_HINSTANCE), "PIXXYPOPUPMENU" );
			if( hPopupMenu == NULL ) return 0;

			TrackPopupMenu( GetSubMenu(hPopupMenu, 0 ),
				             TPM_BOTTOMALIGN, ptOnScreen.x, ptOnScreen.y, 0, hImageWnd, NULL);

			DestroyMenu(hPopupMenu);
			break;

		case WM_NOTIFY: //Messages sent to the parent window from controls

			switch( ((LPNMHDR)lp)->code )
			{
			   case TTN_NEEDTEXT: // tooltips

				   lpttt = (LPTOOLTIPTEXT)lp;
				   lpttt->hinst = (HINSTANCE)GetWindowLong(hImageWnd, GWL_HINSTANCE);

				   switch( lpttt->hdr.idFrom ) // process acoording to control IDs
				   {
				     case IDM_OPEN:
						 lpttt->lpszText = MAKEINTRESOURCE( IDS_OPEN );
					     break;

					 case IDM_CLOSE:
						 lpttt->lpszText = MAKEINTRESOURCE( IDS_CLOSE );
						 break;

					 case IDM_RELOAD:
						 lpttt->lpszText = MAKEINTRESOURCE( IDS_RELOAD );
						 break;

					 case IDM_ENLARGE:
						 lpttt->lpszText = MAKEINTRESOURCE( IDS_ENLARGE );
						 break;

					 case IDM_SHRINK:
						 lpttt->lpszText = MAKEINTRESOURCE( IDS_SHRINK );
						 break;

					 default:
						  break;
				   }

				   break;

			   default:
				  break;
			}

			break;

		default:
			return ( DefWindowProc(hImageWnd, msg, wp, lp) );
	}

	return 0;
}


/*
// Function:		loadimage_2_offscreen( HWND hImageWnd, const char* FileName  )
// Arguments:		HWND	hImageWnd  ...   image display window handle
//					const char* FileName ... image file name
// Return value:    If the function succeeds, the return value is 1.
//                  If the function fails, the return value is zero.
// Description:     The function loads the image on the memory device context (hdcmem).
// Usage:
//
*/
int loadimage_2_offscreen( HWND hImageWnd, const char* FileName  )
{
	     HDC  hdc;

		 // Variables to swap R and B components
		 int i;
		 unsigned char c1, c3;

		 // Load a image with stb
		 pixels = stbi_load ( FileName, &w, &h, &bpp, 4);

		 if( pixels == NULL ) 
		 {
			 MessageBox( hImageWnd, "Can't open this file !", "PIXXY", MB_ICONEXCLAMATION | MB_OK );
			 return 0;
		 }

		 // Convert BGR to RGB
		 for( i = 0; i < 4*w*h; i += 4 )
		 {
			 c1 = *(pixels + i );    // R component
			 c3 = *(pixels + i + 2 ); // B component

			 // swap B with R
			 *(pixels + i ) = c3;
			 *(pixels + i + 2) = c1;
		 }

         b.biPlanes=1;
         b.biBitCount=32;
         b.biWidth = w;
         b.biHeight = -h;

		 hdc = GetDC(hImageWnd);

		  hBitmap = CreateCompatibleBitmap( hdc, w, h );
          hdcMem = CreateCompatibleDC (hdc) ;
		  SelectObject(hdcMem, hBitmap);

          SetDIBitsToDevice( hdcMem, 0, 0,
                               w, h, 
							   0, 0, 0, h,
							   pixels, (BITMAPINFO *) &b, DIB_RGB_COLORS);
		  
		  DeleteObject(hBitmap);
		  ReleaseDC(hImageWnd, hdc);

	return 1;
}

/*
// Function:		LoadStbImage( HWND hDialog, const char* FileName )
// Arguments:		HWND	hDialog  ...   The window handle of the open dialog
//					const char* FileName ... image file name
// Return value:    If the function succeeds, the return value is the bitmap handle.
//                  If the function fails, the return value is NULL.
// Description:     Somthing like the LoadImage fuction in WIN32 API
// Usage:
//
*/
HBITMAP LoadStbImage( HWND hDialog, const char* FileName )
{
	HDC               hdcdialog;
	unsigned char*    pix = NULL;
	int               bpn;
	BITMAPINFOHEADER  bih = { sizeof(bih) };
    HBITMAP			  hBmp = NULL;
	int               arg_w, arg_h;
	
    // Variables to swap R with B.
    int i;
	unsigned char comp1, comp3;

	// Load a image with stb.
	pix = stbi_load ( FileName, &arg_w, &arg_h, &bpn, 4);

	if( pix == NULL ) 
		return NULL;

	 // Convert BGR to RGB
	 for( i = 0; i < 4*(arg_w)*(arg_h); i += 4 )
	 {
		comp1 = *(pix + i );    // B component
		comp3 = *(pix + i + 2 ); // R component

		// swap B with R
		*(pix + i ) = comp3;
		*(pix + i + 2) = comp1;
	 }

     // hBmp = CreateBitmap( arg_w, arg_h, bpn, 32, pix); // It does NOT work for this case.

	  bih.biPlanes=1;
      bih.biBitCount=32;
      bih.biWidth = arg_w;
      bih.biHeight = -arg_h;

      hdcdialog = GetDC(hDialog);

	  hBmp = CreateDIBitmap( hdcdialog, &bih, CBM_INIT, pix, (BITMAPINFO *) &bih, DIB_RGB_COLORS );

	  ReleaseDC(hDialog, hdcdialog);

	 return hBmp;
  
}


int   Open_A_imagefile( HWND hImageWnd, const char* pstrFileName )
{
	 OPENFILENAME ofn;
	 TCHAR szFilter[] = TEXT ("Photoshop Files (*.psd)\0*.psd\0") \
                               TEXT ("JPEG Files (*.jpg)\0*.jpg\0") \
                               TEXT ("All Files (*.*)\0*.*\0\0") ;

	     ofn.lStructSize       = sizeof (OPENFILENAME) ;
         ofn.hwndOwner         = hImageWnd ;
         ofn.hInstance         = (HINSTANCE)GetWindowLong(hImageWnd,GWL_HINSTANCE)  ;
         ofn.lpstrFilter       = szFilter ;
         ofn.lpstrCustomFilter = NULL ;
         ofn.nMaxCustFilter    = 0 ;
         ofn.nFilterIndex      = 0 ;
         ofn.nMaxFile          = MAX_PATH ;
         ofn.nMaxFileTitle     = MAX_PATH ;
         ofn.lpstrInitialDir   = NULL ;
         ofn.lpstrTitle        = NULL ;
         ofn.nFileOffset       = 0 ;
         ofn.nFileExtension    = 0 ;
         ofn.lpstrDefExt       = TEXT ("psd") ;
         ofn.lCustData         = 0L ;
         ofn.lpfnHook          = (LPOFNHOOKPROC)ExplorerHook;
         ofn.lpTemplateName    = MAKEINTRESOURCE(IDD_COMDLG);
	     ofn.lpstrFile         = (PTSTR)pstrFileName ;
         ofn.lpstrFileTitle    = (PTSTR)pstrTitleName ;
         ofn.Flags             = OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
   
         if(!GetOpenFileName (&ofn)) return 0 ;

		 if( loadimage_2_offscreen( hImageWnd, pstrFileName ) == 0 ) return 0;

		// Display File name on the title bar.
		strcpy( szWindowTitle, "pixxy - " );
		strcat( szWindowTitle, pstrTitleName );

		// Display magnifying power on the title bar
		strcat( szWindowTitle, " - " );
		sprintf( szMag, "%3.1f", mag*100 );
		strcat( szWindowTitle, szMag );
		strcat( szWindowTitle, " \%" );

		SetWindowText( hImageWnd, szWindowTitle );
		 
	return 1;
}



/*
// Function:		On_paint( HDC hdc )
// Arguments:		HDC  hdc  ...   The device context of the visible screen to be drawn.
// Return value:    none
// Description:     The function "BitBlts" the image of the memory device context (hdcmem) to the visible screen (hdc).
// Usage:           Put the function in WM_PAINT, WM_SIZE, WM_HSCROLL and WM_VSCROLL.
//
*/
void On_paint( HDC hdc )
{
	SetWindowOrgEx(hdc, org.x, org.y, NULL);
	SetStretchBltMode(hdc, HALFTONE );
	StretchBlt( hdc, 0, PAGER_HEIGHT, mag*w, mag*h, hdcMem, 0, 0, w, h, SRCCOPY);
}


/*
// Function:		void	Scroll_on_redraw(HWND	hImageWnd)
// Arguments:		HWND	 hImageWnd  ...   The handle of the window to be drawn.
// Return value:    none
// Description:     Set the scrollbar size to fit the client area size and image size when the image is redrawn. 
// Usage:           Put the function in WM_PAINT and WM_SIZE.
//
*/
void	Scroll_on_redraw(HWND	hImageWnd)
{
	if( (w != 0) && (h != 0) ) // A image is loaded
	{
		// Retrieve the size of the client area
		GetClientRect(hImageWnd, &rc);

		// Set the scrollbar info according to the loaded image size.
		si.fMask = SIF_RANGE | SIF_PAGE;

		// Maximum scroll range (si.nMax) is the image size minus 1.
		// Page size (si.nPage) is same as the ciant area size.
		si.nMax = mag*w - 1;
		si.nMin = 0;
		si.nPage = rc.right - rc.left;
		SetScrollInfo(hImageWnd, SB_HORZ, &si, TRUE); // Set the horizontal scrollbar.

		// si.nMax = h - 1;
		si.nMax = mag*h + PAGER_HEIGHT + STATUSBAR_HEIGHT - 1;
		si.nMin = 0;
		si.nPage = rc.bottom - rc.top;
		SetScrollInfo(hImageWnd, SB_VERT, &si, TRUE); // Set the vertical scrollbar.

		// Fill the global variable "org" with the current scrollbox position .
		si.fMask = SIF_POS;
		GetScrollInfo(hImageWnd, SB_HORZ, &si);
		org.x = si.nPos;
		GetScrollInfo(hImageWnd, SB_VERT, &si);
		org.y = si.nPos;
	}
}


/*
// Function:		void	Horiz_scroll(HWND	hImageWnd, WPARAM wp)
// Arguments:		HWND	 hImageWnd  ...   The handle of the window to be drawn.
// Return value:    none
// Description: 
// Usage:           Put the function in WM_HSCROLL.
//
*/
void	Horiz_scroll(HWND	hImageWnd, WPARAM wp)
{
	si.fMask = SIF_ALL;
	GetScrollInfo(hImageWnd, SB_HORZ, &si);

	iHorzPos = si.nPos; // Conserve the current scrollbox position

	switch( LOWORD(wp) )
	{
		case SB_ENDSCROLL: // when the mouse button is released relating to the scrollbar
			break;			
		case SB_LEFT: // when the scrollbox is on the left edge
					// Not need to process according to the Petzold's book
			si.nPos = si.nMin;
			break;
		case SB_RIGHT:	// when the scrollbox is on the right edge
			si.nPos = si.nMax;
			break;
		case SB_LINELEFT:	// click the left scroll arrow button
			si.nPos -= 8;
			break;
		case SB_LINERIGHT:	// click the right scroll arrow button
			si.nPos += 8;
			break;
		case SB_PAGELEFT:	// click the shaft to the left of the scroll box
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:	// click the shaft to the right of the scroll box
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:		// hold the scrollbox
		case SB_THUMBPOSITION:	// release the scrolllbox
			si.nPos = si.nTrackPos;
			break;
		}

	// Set si.Pos according to the change by the user
	si.fMask = SIF_POS;
	SetScrollInfo(hImageWnd, SB_HORZ, &si, TRUE);

	// Fill the global "org" with the current scrollbox position
	GetScrollInfo(hImageWnd, SB_HORZ, &si);
	org.x = si.nPos;

	// Scroll image along the horizontal
	ScrollWindowEx(hImageWnd, iHorzPos - si.nPos, 0, NULL, NULL, NULL, NULL, SW_INVALIDATE);

}


/*
// Function:		void	Vert_scroll(HWND	hImageWnd, WPARAM wp)
// Arguments:		HWND	 hImageWnd  ...   The handle of the window to be drawn.
// Return value:    none
// Description: 
// Usage:           Put the function in WM_VSCROLL.
//
*/
void	Vert_scroll(HWND	hImageWnd, WPARAM wp)
{
	si.fMask = SIF_ALL;
	GetScrollInfo(hImageWnd, SB_VERT, &si);

	iVertPos = si.nPos;

	switch( LOWORD(wp) )
	{
		case SB_ENDSCROLL: // when the mouse button is released relating to the scrollbar
			break;			
		case SB_TOP: // when the scrollbox is on the top
					// Not need to process according to the Petzold's book
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:	// when the scrollbox is on the bottom
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:	// click the top scroll arrow
			si.nPos -= 8;
			break;
		case SB_LINEDOWN:	// click the bottom scroll arrow
			si.nPos += 8;
			break;
		case SB_PAGEUP:	// click the shaft above the scrollbox
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:	// click the shaft below the scrollbox
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:		// hold the scrollbox
		case SB_THUMBPOSITION:	// release the scrollbox
			si.nPos = si.nTrackPos;
			break;
	}

	// Set si.Pos according to the change by the user
	si.fMask = SIF_POS;
	SetScrollInfo(hImageWnd, SB_VERT, &si, TRUE);

	// Fill the global "org" with the current scrollbox position
	GetScrollInfo(hImageWnd, SB_VERT, &si);
	org.y = si.nPos;

	// Scroll the image along the vertical
	ScrollWindowEx(hImageWnd, 0, iVertPos - si.nPos, NULL, NULL, NULL, NULL, SW_INVALIDATE);

}


double set_magPower( int mp )
{
	switch( mp )
	{
	   case lifesize:
	   default:
		   mag = 1.00000;
		   break;

	   case one_of_8:
		   mag = 0.12500;
		   break;

	   case one_of_6:
		   mag = 0.16667;
		   break;

	   case one_of_4:
		   mag = 0.25000;
		   break;

	   case third:
		   mag = 0.33333;
		   break;

	   case half:
		   mag = 0.50000;
		   break;

	   case two_of_3:
		   mag = 0.66667;
		   break;

	   case twice:
		   mag = 2.00000;
		   break;

	   case triple:
		   mag = 3.00000;
		   break;

	   case four_times:
		   mag = 4.00000;
		   break;

	   case five_times:
		   mag = 5.00000;
		   break;

	   case six_times:
		   mag = 6.00000;
		   break;

	   case seven_times:
		   mag = 7.00000;
		   break;
	}

	return mag;
}

int    get_pixel_coord_on_mouse( HWND hImageWnd, int* lpX, int* lpY )
{
	POINT  pt;

	if( pixels == NULL )
	{
		*lpX = 0;
		*lpY = 0;

		return 0;
	}

	GetCursorPos( &pt );
    ScreenToClient(hImageWnd, &pt );

	if( ( (double)(pt.x) > (double)(w)*mag ) || ( (double)(pt.y) - PAGER_HEIGHT > (double)(h)*mag ) )
	{
		*lpX = 0;
		*lpY = 0;

		return 0;
	}

	*lpX = round( (double)(pt.x + org.x)/mag );
	*lpY = round( (double)(pt.y + org.y + 2 - PAGER_HEIGHT ) /mag ); // +2 is border width correction

	return 1;
}

int  round(double value) // Round up numbers of five and above, and round down anything under five (shisha-gonyu in Japanese).
{
	double greater;
	double lesser;
	double decimal;
	
	greater = ceil(value);
	lesser = floor(value);

	if( value >= 0.0 )
	{
		decimal = value - lesser;

		if( decimal >= 0.5 ) return (int)greater;
         else return (int)lesser;
	}

	else 
	{
		decimal = greater - value;

	    if( decimal >= 0.5 ) return (int)lesser;
         else return (int)greater;
	}
}



BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT msg, WPARAM wp, LPARAM lp )
{
	switch(msg)
	{
	    case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			switch( LOWORD(wp) )
			{
			    case IDOK:
					EndDialog(hDlg, 0);
					return FALSE;
			}

		break;
	}

	return FALSE;
}





/*
// Function：		BOOL CALLBACK ExplorerHook(HWND, UINT, WPARAM, LPARAM)
// Arguments：		Same as window procedure
// Return value：
// Description：	Process messages sent to the open dialog
// Usage：	
*/
BOOL CALLBACK ExplorerHook(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
         // Turn the checkbox on.
         SendMessage(GetDlgItem(hDlg,IDD_COM_CHECK),BM_SETCHECK,1,0); 
         break;
    case WM_COMMAND:
    {
      HDC     hDC;
      RECT    Rect;
      HBRUSH  hBrsuh;
     
      if (LOWORD(wParam)==IDD_COM_CHECK) 
      { 
		 // Clear the preview image if the checkbox is off.
        if (SendMessage(GetDlgItem(hDlg,IDD_COM_CHECK),BM_GETCHECK,0,0)!=1)     
        {
            hDC=GetDC(GetDlgItem(hDlg,IDD_COM_IMAGE));
			// Paint over the preview area.
            GetClientRect(GetDlgItem(hDlg,IDD_COM_IMAGE),&Rect);
			// Resize on accordance with the frame
            Rect.left=Rect.left+2;Rect.top=Rect.top+2;Rect.right=Rect.right-2;Rect.bottom=Rect.bottom-2;
			// Create brush
            hBrsuh=CreateSolidBrush(RGB(192,192,192));
            FillRect(hDC,&Rect,hBrsuh);
            DeleteObject(hBrsuh);
            ReleaseDC(GetDlgItem(GetParent(hDlg),IDD_COM_IMAGE),hDC);
			// Change the text
            SendMessage(GetDlgItem(hDlg,IDD_COM_STATIC),WM_SETTEXT,0,(LPARAM)"Image:");
        }
      }
      break;
    }
    case WM_PAINT:
    {
        OFNOTIFY OFN;

		// Redraw
        OFN.hdr.code=CDN_SELCHANGE;
        OnNotify(hDlg, &OFN);
        return FALSE;
        break;
    }
    case WM_NOTIFY:
         OnNotify(hDlg, (LPOFNOTIFY)lParam);
    default:
         return FALSE;
    }
    return TRUE;
}


/*
// Function：		OnNotify(HWND hDlg, LPOFNOTIFY pOFN)
// Arguments：		HWND	hDlg	 ...	The handle of the open dialogbox.
//                  LPOFNOTIFY pOFN   ...   Pointer to the OFNOTIFY structure.
// Return value：	
// Description：	Process WM_NOTYFY message sent to open dialogbox.
// Usege：
*/
BOOL NEAR PASCAL OnNotify(HWND hDlg, LPOFNOTIFY pOFN)
{
    switch (pOFN->hdr.code)
    {

    case CDN_INITDONE:
    {
         RECT Rect;
        
         // Centering the open dialog
         GetWindowRect((HWND)GetWindowLong(hDlg, GWL_HWNDPARENT), &Rect);
         SetWindowPos((HWND)GetWindowLong(hDlg, GWL_HWNDPARENT), 0,((GetSystemMetrics(SM_CXSCREEN) - Rect.right + Rect.left) / 2),
                         (GetSystemMetrics(SM_CYSCREEN) - Rect.bottom + Rect.top) / 3,0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
         break;
    }

    case CDN_SELCHANGE:
    {
            char szFile[MAX_PATH];
            char buffer[100];
            HBITMAP hBmp=NULL;
			HBITMAP hDummy=NULL;
            BITMAP  BMPINFO;
            HDC     hMem,hDC;
			RECT    Rect;       // Preview area
            HBRUSH  hBrsuh;
            int     Width,Height,Top,Left; // area to draw preview image

			// Make sure that sizeof(szFile) is less than the limit.
            if (SendMessage(GetParent(hDlg), CDM_GETSPEC,(WPARAM)sizeof(szFile),(LPARAM)(LPWSTR)szFile)<=sizeof(szFile));
            {  
				// Load image if the checkbox is on.
                if (SendMessage(GetDlgItem(hDlg,IDD_COM_CHECK),BM_GETCHECK,0,0)==1) 
                {
					// Load image with stb
					hBmp = LoadStbImage( hDlg, szFile );

                    if (hBmp!=NULL)
                    {   
                        hDC = GetDC(GetDlgItem(hDlg,IDD_COM_IMAGE));
                        GetWindowRect(GetDlgItem(hDlg,IDD_COM_IMAGE), &Rect);
                        GetObject(hBmp, sizeof(BITMAP), &BMPINFO);
                        hMem = CreateCompatibleDC(hDC);
                        SelectObject(hMem, hBmp);
                        Width  = Rect.right-Rect.left;
                        Height = Rect.bottom-Rect.top;

						// Display image size on the open dialog
						wsprintf(buffer,"Image: %d x %d",BMPINFO.bmWidth,BMPINFO.bmHeight);
                        SendMessage(GetDlgItem(hDlg,IDD_COM_STATIC),WM_SETTEXT,0,(LPARAM)buffer);
                        
                        // Get the preview area info and put it in Rect variable.
                        GetClientRect(GetDlgItem(hDlg,IDD_COM_IMAGE),&Rect);

						// Resize on accordance with the frame
                        Rect.left = Rect.left+2;
						Rect.top = Rect.top+2;
						Rect.right = Rect.right-2;
						Rect.bottom = Rect.bottom-2;

						// Create brush and paint over the preview area
                        hBrsuh = CreateSolidBrush(RGB(192,192,192));
                        FillRect(hDC,&Rect,hBrsuh);
                        DeleteObject(hBrsuh);

						SetStretchBltMode(hDC, COLORONCOLOR);
                       					
						// Two way to center the preview image
						// In the case that loaded image is smaller than the preview area.
                        if ((BMPINFO.bmWidth<Width+(int)(Height*0.1)) && (BMPINFO.bmHeight<Height)+(int)(Height*0.1))
                        {  
                           Left = (int)((Width-BMPINFO.bmWidth)/2);
                           Top = (int)((Height-BMPINFO.bmHeight)/2);
                           BitBlt(hDC,Left,Top, BMPINFO.bmWidth,BMPINFO.bmHeight, hMem, 0, 0, SRCCOPY);
                        }
                        else
                        {
                          Top = (int)(Height*0.1);
                          Left = (int)(Width*0.1);

                          StretchBlt (hDC,Left,Top,(int)(Width*0.8),(int)(Height*0.8),hMem,0,0,BMPINFO.bmWidth,BMPINFO.bmHeight,SRCCOPY);

                        }

                        DeleteDC(hMem);    
                        DeleteObject(hBmp);
                        ReleaseDC(GetDlgItem(GetParent(hDlg),IDD_COM_IMAGE),hDC);                    
                    }
                }
            }
    
        }
        break;
    }
    return(TRUE);
}