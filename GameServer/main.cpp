#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "GameServer.h"
#include "Room.h"
#include "DBBind.h"
#include "DBConnectionPool.h"

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GameServer* gameServer;

//BOOL CtrlHandler(DWORD CtrlType)
//{
//	switch (CtrlType)
//	{
//		// Handle the CTRL+C signal.
//	case CTRL_C_EVENT:
//	case CTRL_CLOSE_EVENT: // CTRL+CLOSE: confirm! that the user wants to exit.
//	case CTRL_BREAK_EVENT:
//	case CTRL_LOGOFF_EVENT:
//	case CTRL_SHUTDOWN_EVENT:
//	default:
//		gameServer->Close();
//		delete gameServer;
//		gameServer = nullptr;
//		return FALSE; 
//	}
//}

//	TEMP

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(6519);
	std::cout.imbue(std::locale("kor"));
	std::wcout.imbue(std::locale("kor"));

	//ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};server=(localdb)\\MSSQLLocalDB;database=GameServerDb;trusted_connection=Yes;"));

	//	Exit Function Callback Register
	/*BOOL successd = SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	ASSERT_CRASH(successd);*/

	{
		gameServer = new GameServer();

		ASSERT_CRASH(gameServer->Initialize());
		ASSERT_CRASH(gameServer->Start());
		gameServer->DoProcessJob();
	}

	delete gameServer;

	system("pause");

	return 0;
}


/*
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
#include "Profiler.h"

// Main code
int main(int, char**)
{
	{
		GProfiler.Initialize(5, 1024);
	}

	::glfwSetErrorCallback(glfw_error_callback);
	if (!::glfwInit())
		return 1;
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

	// Create window with graphics context
	GLFWwindow* window = ::glfwCreateWindow(1280, 720, "Server Profiler", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	::glfwMakeContextCurrent(window);
	::glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	::ImGui_ImplGlfw_InitForOpenGL(window, true);
	::ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!::glfwWindowShouldClose(window))
	{
		PROFILE_FRAME();

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		::glfwPollEvents();

		// Start the Dear ImGui frame
		::ImGui_ImplOpenGL3_NewFrame();
		::ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
		{
			PROFILE_SCOPE("Demo");
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		::glfwGetFramebufferSize(window, &display_w, &display_h);
		::glViewport(0, 0, display_w, display_h);
		::glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		::glClear(GL_COLOR_BUFFER_BIT);
		::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		::glfwSwapBuffers(window);
	}

	// Cleanup
	::ImGui_ImplOpenGL3_Shutdown();
	::ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	::glfwDestroyWindow(window);
	::glfwTerminate();
	
	return 0;
}

*/


















/*
{
	//	DB Test
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};server=(localdb)\\MSSQLLocalDB;database=GameServerDb;trusted_connection=Yes;"));

	//	Create Table
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY, \
				[gold] INT NULL,						\
				[name] NVARCHAR(50) NULL,				\
				[createDate] DATETIME NULL				\
			);";

		auto dbConntion = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConntion->Execute(query));
		GDBConnectionPool->Push(dbConntion);
	}

	{
		for (int32 i = 0; i < 3; i++)
		{
			auto dbConntion = GDBConnectionPool->Pop();
			DBBind<3, 0> dbBind(*dbConntion, L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");

			int32 gold = 100;
			dbBind.BindParam(0, gold);
			WCHAR name[100] = L"ȫ�浿";
			dbBind.BindParam(1, name);
			TIMESTAMP_STRUCT ts = { 2024,5,18 };
			dbBind.BindParam(2, ts);
			ASSERT_CRASH(dbBind.Execute());

			GDBConnectionPool->Push(dbConntion);
		}
	}

	{
		auto dbConntion = GDBConnectionPool->Pop();
		DBBind<1, 4> dbBind(*dbConntion, L"SELECT id, gold, name, createDate FROM [dbo].[gold] WHERE gold = (?)");
		int32 gold = 100;
		dbBind.BindParam(0, gold);

		int32 outId = 0;
		int32 outGold = 0;
		WCHAR outName[100] = { 0, };
		TIMESTAMP_STRUCT outTs = { 0, };
		dbBind.BindCol(0, OUT outId);
		dbBind.BindCol(1, OUT outGold);
		dbBind.BindCol(2, OUT outName);
		dbBind.BindCol(3, OUT outTs);

		ASSERT_CRASH(dbBind.Execute());

		wcout.imbue(std::locale("kor"));

		while (dbBind.Fetch())
		{
			wcout << "id : " << outId << " Gold : " << outGold << " Name : " << outName << endl;
			wcout << "Date : " << outTs.year << "/" << outTs.month << "/" << outTs.day << endl;
		}

		GDBConnectionPool->Push(dbConntion);
	}
}


GRoom->PushReserveJob(1000, []()
	{
		cout << 1000 << endl;
	}
);
GRoom->PushReserveJob(2000, []() {cout << 2000 << endl; });
GRoom->PushReserveJob(3000, []() {cout << 3000 << endl; });
*/