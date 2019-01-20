#include <string.h>
#include <locale.h>
#include <interfaces/IInput.h>
#include <interfaces/ISchemeManager.h>
#include <interfaces/ISurface.h>
#include <interfaces/ISystem.h>
#include <interfaces/IVGui.h>
#include <interfaces/IPanel.h>
#include <interfaces/ILocalize.h>
#include <interfaces/IKeyValues.h>
#include <FileSystem.h>
#include "controls.h"

namespace vgui2
{
	vgui2::IInput *g_pInputInterface = NULL;
	vgui2::ISchemeManager *g_pSchemeInterface = NULL;
	vgui2::ISurface *g_pSurfaceInterface = NULL;
	vgui2::ISystem *g_pSystemInterface = NULL;
	vgui2::IVGui *g_pVGuiInterface = NULL;
	vgui2::IPanel *g_pPanelInterface = NULL;
	vgui2::ILocalize *g_pLocalizeInterface = NULL;
	vgui2::IKeyValues *g_pKeyValuesInterface = NULL;
	IFileSystem *g_pFileSystemInterface = NULL;

	vgui2::IInput *input() {
		return g_pInputInterface;
	}

	vgui2::ISchemeManager *scheme() {
		return g_pSchemeInterface;
	}

	vgui2::ISurface *surface() {
		return g_pSurfaceInterface;
	}

	vgui2::ISystem *system() {
		return g_pSystemInterface;
	}
	
	vgui2::IVGui *ivgui() {
		return g_pVGuiInterface;
	}

	vgui2::IPanel *ipanel() {
		return g_pPanelInterface;
	}

	vgui2::ILocalize *localize() {
		return g_pLocalizeInterface;
	}

	vgui2::IKeyValues *keyvalues() {
		return g_pKeyValuesInterface;
	}
	
	IFileSystem *filesystem() {
		return g_pFileSystemInterface;
	}

	static void *InitializeInterface(char const *interfaceName, CreateInterfaceFn *factoryList, int numFactories) {
		void *retval;

		for (int i = 0; i < numFactories; i++) {
			CreateInterfaceFn factory = factoryList[i];
			if (!factory)
				continue;

			retval = factory(interfaceName, NULL);
			if (retval)
				return retval;
		}

		return NULL;
	}

	static char g_szControlsModuleName[256];

	bool VGuiControls_Init(const char *moduleName, CreateInterfaceFn *factoryList, int numFactories) {
		strncpy_s(g_szControlsModuleName, moduleName, sizeof(g_szControlsModuleName));
		g_szControlsModuleName[sizeof(g_szControlsModuleName) - 1] = 0;

		setlocale(LC_CTYPE, "");
		setlocale(LC_TIME, "");
		setlocale(LC_COLLATE, "");
		setlocale(LC_MONETARY, "");

		g_pVGuiInterface = (IVGui *)InitializeInterface(VGUI_IVGUI_INTERFACE_VERSION, factoryList, numFactories);
		g_pPanelInterface = (IPanel *)InitializeInterface(VGUI_PANEL_INTERFACE_VERSION, factoryList, numFactories);
		g_pSurfaceInterface = (ISurface *)InitializeInterface(VGUI_SURFACE_INTERFACE_VERSION, factoryList, numFactories);
		g_pSchemeInterface = (ISchemeManager *)InitializeInterface(VGUI_SCHEME_INTERFACE_VERSION, factoryList, numFactories);
		g_pSystemInterface = (ISystem *)InitializeInterface(VGUI_SYSTEM_INTERFACE_VERSION, factoryList, numFactories);
		g_pInputInterface = (IInput *)InitializeInterface(VGUI_INPUT_INTERFACE_VERSION, factoryList, numFactories);
		g_pLocalizeInterface = (ILocalize *)InitializeInterface(VGUI_LOCALIZE_INTERFACE_VERSION, factoryList, numFactories);
		g_pFileSystemInterface = (IFileSystem *)InitializeInterface(FILESYSTEM_INTERFACE_VERSION, factoryList, numFactories);
		g_pKeyValuesInterface = (IKeyValues *)InitializeInterface(VGUI_KEYVALUES_INTERFACE_VERSION, factoryList, numFactories);

		if (!g_pVGuiInterface) {
			return false;
		}

		if (g_pKeyValuesInterface) {
			// TODO: implement KeyValues
			g_pKeyValuesInterface->RegisterSizeofKeyValues(24);
		}

		g_pVGuiInterface->Init(factoryList, numFactories);

		if (g_pSchemeInterface &&
			g_pSurfaceInterface &&
			g_pSystemInterface &&
			g_pInputInterface &&
			g_pVGuiInterface &&
			g_pFileSystemInterface &&
			g_pLocalizeInterface &&
			g_pPanelInterface)
			return true;

		return false;
	}

	const char *GetControlsModuleName() {
		return g_szControlsModuleName;
	}
}
