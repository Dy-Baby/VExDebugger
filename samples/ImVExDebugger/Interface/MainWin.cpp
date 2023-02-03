#include "MainWin.h"
#include <map>
#include <sstream>
#include <VExDebugger.h>
#include "CustomConfig.hpp"
#include <string>
#include "../Utils/Utils.h"
#include <stb_image.h>

//// for test
//std::map<int, ExceptionAddressCount> ExcpAssocTests =
//{
//    {0, 
//        { 
//            { reinterpret_cast<void*>( 0x111111 ), 
//                {
//                    145, 15888, {}
//                },
//            }, 
//            { reinterpret_cast<void*>( 0x1d1112 ),
//                {
//                    478, 12088, {}
//                },
//            },
//            { reinterpret_cast<void*>( 0x1d1113 ),
//                {
//                    125, 32898, {}
//                },
//            },
//            { reinterpret_cast<void*>( 0x1d1114 ),
//                {
//                    4825, 13213, {}
//                },
//            },
//            { reinterpret_cast<void*>( 0x1d1115 ),
//                {
//                    941, 15888, {}
//                },
//            },
//            { reinterpret_cast<void*>( 0x1d1116 ),
//                {
//                    325, 148744, {}
//                },
//            },
//        } 
//    },
//    {2, 
//        { 
//            { reinterpret_cast<void*>( 0xAAAAAAA ),
//                {
//                    941, 15888, {}
//                },
//            },
//    
//            { reinterpret_cast<void*>( 0x9999999 ),
//                {
//                    941, 15888, {}
//                },
//            },
//
//        } 
//    },
//    {4,  
//        {
//            { reinterpret_cast<void*>( 0xA919999 ),
//                {
//                    941, 15888, {}
//                },
//            },
//        }
//    },
//
//};
//std::map<int, uintptr_t> AddressAssocList =
//{
//    {0, 0x158976A},
//    {2, 0xC580061},
//    {4, 0},
//    {8, 0}
//};

void Gui::Main( GLWindow* Instance, bool * pVisible )
{
    static bool once = true;

    if ( once )
    {
        once = false;

        Instance->SetFramePos( 0.f, 0.f );

        Instance->SetSize( 370, 500 );

        Instance->Center( );
    }

    ImGui::SetNextWindowPos( Instance->GetFramePos(), ImGuiCond_FirstUseEver );

    ImGui::SetNextWindowSize( Instance->GetSize() );

    auto WindowFlags = 0;

    WindowFlags |= ImGuiWindowFlags_NoScrollbar;
    WindowFlags |= ImGuiWindowFlags_NoMove;
    WindowFlags |= ImGuiWindowFlags_NoResize;
    WindowFlags |= ImGuiWindowFlags_NoCollapse;

    if ( ImGui::Begin( "VExDebugger", pVisible, WindowFlags ) )
    {
        static auto SaveLogs = false;
        ImGui::TextCustom( ImVec2( 10.f, 30.f ), { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }, true, "VExDebugger" );

        ImGui::NewLine( );

        ImGui::NewLine( );

        ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 3.0f );

        ImGui::PushStyleVar( ImGuiStyleVar_Alpha, 1.0f );

        ImGui::BeginChild( "#panel", { Instance->GetSize( ).x - 16.f, 350.f }, true );

        const ImGuiTabBarFlags TabBarFlags = ImGuiTabBarFlags_None;

        if ( ImGui::BeginTabBar( "#TabBar", TabBarFlags ) )
        {
            auto inc = 0;

            for ( const auto& [ Address, BpInfo ] : VExDebugger::GetBreakpointList( ) )
            {
                if ( !Address )
                    continue;

                if ( BpInfo.Type != BkpType::Hardware )             // only support hardware breakpoint
				    continue;

			    auto ItExceptionList  = VExDebugger::GetAssocExceptionList( ).find( Address );

                if ( ItExceptionList == VExDebugger::GetAssocExceptionList( ).end( ) )
                    continue;

                auto& ExceptionList     = ItExceptionList->second;

                auto const HexStr       = "0x" + Utils::ValToHexStr( Address );

                if ( ImGui::BeginTabItem( ( std::to_string( ++inc ) + " | " + HexStr ).c_str( ) ) )
                {
                    auto NumItems = ExceptionList.size( );
                       
                    if ( ImGui::Button( ( "Remove " + HexStr ).c_str() , { 320.f, 25.f } ) )
                        VExDebugger::RemoveMonitorAddress( Address );

                    std::string SaveLogsStr{};

                    if ( SaveLogs )
                        SaveLogsStr.append( "\n# List for " + HexStr + "\n" );

                    for ( const auto& [ExceptionAddress, ExceptionInfo] : ExceptionList )
                    {
                        ImGui::PushID( 18 * NumItems );
                        const auto hue = NumItems * 0.05f;
                        ImGui::PushStyleColor( ImGuiCol_ButtonHovered,  static_cast<ImVec4>( ImColor::HSV( hue, 0.7f, 0.7f ) ) );
                        ImGui::PushStyleColor( ImGuiCol_ButtonActive,   static_cast<ImVec4>( ImColor::HSV( hue, 0.8f, 0.8f ) ) );

                        char format[ 100 ]{};
                        sprintf_s( format, sizeof( format ), "Count %8d Address: 0x%p", (int)ExceptionInfo.Details.Count, ExceptionAddress );

                        if ( SaveLogs )
                            SaveLogsStr.append( std::string( format ) + "\n" );
                            
                        if ( ImGui::Button( format, ImVec2( 320.f, 20.f ) ) )
                            printf( "clicked 0x%p\n", ExceptionAddress );
                            
                        ImGui::PopStyleColor( 2 );
                        ImGui::PopID( );

                        ++NumItems;
                    }

                    if ( SaveLogs )
                    {
                        Utils::CreateFileFromText( L"VExDebugger_" + std::wstring( HexStr.begin(), HexStr.end() ) + L".log", SaveLogsStr );
                        SaveLogsStr.clear( );
                    }

                    ImGui::EndTabItem( );
                }
            }
            if ( SaveLogs )
            {
                SaveLogs = false;
            }


            ImGui::EndTabBar( );
        }      
        ImGui::EndChild( );

        ImGui::PopStyleVar( );
        ImGui::PopStyleVar( );

        static char BuffAddress[ 100 ]{};
        ImGui::PushID( 5722 );
        ImGui::PushItemWidth( 100.f );
        ImGui::InputText( "", BuffAddress, 100);
        ImGui::PopItemWidth( );
        ImGui::PopID( );

        ImGui::SameLine( 115.f );

        const char* types[] = { "Execute", "Read/Write","Write" };
        const char* sizes[] = { "Byte 1","Byte 2","Byte 8","Byte 4" };
        static int TypeCurrent = 0;
        static int SizeCurrent = 0;

        ImGui::PushID( 5712 );
        ImGui::PushItemWidth( 100.f );
        ImGui::Combo( "", &TypeCurrent, types, IM_ARRAYSIZE( types ) );
        ImGui::PopItemWidth( );
        ImGui::PopID( );

        ImGui::SameLine( 220.f );

        ImGui::PushID( 5710 );
        ImGui::PushItemWidth( 75.f );
        ImGui::Combo( "", &SizeCurrent, sizes, IM_ARRAYSIZE( sizes ) );
        ImGui::PopItemWidth( );
        ImGui::PopID( );

        ImGui::SameLine( 300.f );

        if ( ImGui::Button( "Add", { 60.f, 20.f } ) )
        {
            std::string StrAddress( BuffAddress );   

            if ( Utils::IsValidHex( StrAddress ) )
            {
                const auto ResultConverted = ( ( sizeof uintptr_t ) < 8 ) ?
                    static_cast<uintptr_t>( strtoul( StrAddress.c_str( ), nullptr, 16 ) ) : static_cast<uintptr_t>( strtoull( StrAddress.c_str( ), nullptr, 16 ) );
                
                if ( ResultConverted )
                    VExDebugger::StartMonitorAddress( ResultConverted, static_cast<HwbkpType>( TypeCurrent ), static_cast<HwbkpSize>( SizeCurrent ) );
            }
        }

        ImGui::NewLine();

        if ( ImGui::Button( "Save logs", { 354.f, 20.f } ) )
            SaveLogs = true;

        ImGui::End( );
    }
}

