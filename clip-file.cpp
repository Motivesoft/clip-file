// clip-file.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <windows.h>

void usage( const char* appName )
{
   std::cout << std::endl;
   std::cout << "Usage:" << std::endl;
   std::cout << "    " << appName << "<filename>" << std::endl;
   std::cout << std::endl;
}

void help( const char* appName )
{
   std::cout << std::endl;
   std::cout << "Copy the contents of a file to the clipboard." << std::endl;

   usage( appName );
}

bool isHelpRequest( const char* argument )
{
   if ( argument != NULL && strlen( argument ) > 1 )
   {
      if ( argument[ 0 ] == '/' || argument[ 0 ] == '-' )
      {
         const char* helpRequests[] =
         {
            "-h",
            "-help",
            "-?",
            "/h",
            "/help",
            "/?",
         };

         for ( int loop = 0; loop < _countof( helpRequests ); loop++ )
         {
            if ( _stricmp( helpRequests[ loop ], argument ) == 0 )
            {
               return true;
            }
         }
      }
   }

   return false;
}

bool process( const char* filename )
{
   if ( ::IsClipboardFormatAvailable( CF_TEXT ) && ::OpenClipboard( NULL ) )
   {
      // Use try...finally to make sure we close the clipboard after use
      //__try
      {
         std::ifstream file( filename );
         std::stringstream buffer;
         buffer << file.rdbuf();

         HANDLE h = GlobalAlloc( GMEM_MOVEABLE,
                                 ( buffer.str().length() + 1 ) * sizeof( TCHAR ) );

         if ( h != NULL )
         {
            LPTSTR v = (LPTSTR) ::GlobalLock( h );

            if ( v != NULL )
            {
               memcpy( v, buffer.str().c_str(), buffer.str().length() * sizeof( TCHAR ) );
               v[ buffer.str().length() ] = (BYTE) '\0';

               ::GlobalUnlock( v );
               ::SetClipboardData( CF_TEXT, h );
            }

            ::GlobalFree( h );
         }
      }
      //__finally
      {
         ::CloseClipboard();
      }
   }
   else
   {
      std::cerr << "Clipboard is inaccessible" << std::endl;
   }

   return true;
}

int main( int argc, char** argv )
{
   // We're going to be showing an message to the user. Reference the executable name when we do this for usage examples
   char appName[ _MAX_FNAME ];
   _splitpath_s( argv[ 0 ], NULL, 0, NULL, 0, appName, _MAX_FNAME, NULL, 0 );
   _strupr_s( appName, _MAX_FNAME );

   bool showHelp = true;

   if ( argc == 2 )
   {
      if ( !isHelpRequest( argv[ 1 ] ) )
      {
         showHelp = false;
         if( !process( argv[ 1 ] ) )
         {
            usage( appName );
         }
      }
   }

   if ( showHelp )
   {
      help( appName );
   }
}
