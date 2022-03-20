// clip-file.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "clip-file.h"

void process( LPWSTR filename )
{
   HANDLE hFile;
   hFile = CreateFile( filename,              // file to open
                       GENERIC_READ,          // open for reading
                       FILE_SHARE_READ,       // share for reading
                       NULL,                  // default security
                       OPEN_EXISTING,         // existing file only
                       FILE_ATTRIBUTE_NORMAL, // normal file
                       NULL );                // no attr. template

   if ( hFile != INVALID_HANDLE_VALUE )
   {
      DWORD extra;
      DWORD dwSize = GetFileSize( hFile, &extra );

      if ( extra > 0 )
      {
         MessageBox( NULL, L"File too large", L"Error", MB_OK );
         CloseHandle( hFile );
         return;
      }

      char* buffer = new char[ dwSize + sizeof( char ) ];

      if ( ReadFile( hFile, buffer, dwSize, NULL, NULL ) )
      {
         if ( ::IsClipboardFormatAvailable( CF_TEXT ) && ::OpenClipboard( NULL ) )
         {
            ::EmptyClipboard();

            HGLOBAL h = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE,
                                     ( (SIZE_T) dwSize + 10 ) );
            if ( h != NULL )
            {
               char* v = ( char* ) ::GlobalLock( h );

               if ( v != NULL )
               {
                  memcpy( v, buffer, dwSize );
                  v[ dwSize ] = '\0';

                  ::GlobalUnlock( v );

                  if ( !::SetClipboardData( CF_TEXT, h ) )
                  {
                     MessageBox( NULL, L"Failed to read file", L"Error", MB_OK );
                  }
               }

               ::GlobalFree( h );
            }

            ::CloseClipboard();
         }
      }
      else
      {
         MessageBox( NULL, filename, L"Failed to read file", MB_OK );
      }

      delete[] buffer;
      CloseHandle( hFile );
   }
}

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR    lpCmdLine,
                       _In_ int       nCmdShow )
{
   UNREFERENCED_PARAMETER( hInstance );
   UNREFERENCED_PARAMETER( hPrevInstance );

   // Extract the filename and remove surrounding quotes if present
   LPWSTR filename = lpCmdLine;
   if ( wcslen( filename ) > 0 )
   {
      if ( filename[ 0 ] == TEXT( '\"' ) )
      {
         filename[ wcslen( filename ) - 1 ] = TEXT( '\0' );
         filename++;
      }
   }

   // Now load the file
   if ( wcslen( filename ) > 0 )
   {
      //MessageBox( NULL, filename, L"Filename", MB_OK );
      process( filename );
   }
   else
   {
      MessageBox( NULL, L"Specify filename as a command line argument and the file contents will be placed onto the clipboard", L"Help", MB_OK );
   }

   return 0;
}
