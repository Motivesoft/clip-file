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
      ::EmptyClipboard();

      // Use try...finally to make sure we close the clipboard after use
      //__try
      {
         std::ifstream t;
         size_t length;
         t.open( filename );      // open input file
         t.seekg( 0, std::ios::end );    // go to the end
         length = t.tellg();           // report location (this is the length)
         t.seekg( 0, std::ios::beg );    // go back to the beginning
         char* buffer = new char[ length + 1 ];    // allocate memory for a buffer of appropriate dimension
         t.read( buffer, length );       // read the whole file into the buffer
         t.close();                    // close file handle

         buffer[ length ] = '\0';

         HGLOBAL h = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE,
                                  ( length + 10 ) );

         if ( h != NULL )
         {
            char* v = (char*) ::GlobalLock( h );

            if ( v != NULL )
            {
               memcpy( v, buffer, length );
               v[ length ] = '\0';

               ::GlobalUnlock( v );

               if ( !::SetClipboardData( CF_TEXT, h ) )
               {
                  std::cerr << "Failed to set text onto clipboard" << std::endl;
               }
            }

            ::GlobalFree( h );
         }
      }

      ::CloseClipboard();
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
