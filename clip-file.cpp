// clip-file.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

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
   else
   {

   }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
