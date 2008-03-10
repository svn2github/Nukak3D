/** 
 * @file nkNukak3D.h
 * @brief Ventana principal de Nukak3D.
 * @details Implementa la interfaz gráfica 
 * @author Alexander Pinzón Fernandez, Byron Pérez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/

#ifndef _NKNUKAK3D_H_
#define _NKNUKAK3D_H_

#include "vtkINRIA3DConfigure.h"
#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

/** wx */
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/joystick.h>
#ifndef __WXMSW__
    #include "mondrian.xpm"
#endif

#include "main.xpm"

/** vtk*/
#include <vtkOutputWindow.h>
#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkTIFFWriter.h>

/** nk*/
#include "nkObj3DViewer.h"
#include "nkVolViewer.h"
#include "nkIODialog.h"
#include "nkAcercaDe.h"

//#ifdef __WXMAC__
#ifdef __WIN32__
	#include "nkToolBar.h"
#else // mac o unix
	#include "nkToolBarMac.h"
#endif //__WIN32__

#define mensajes 1

/**
 * @brief Ventana principal de Nukak3D.
 * @details Implementa la interfaz gráfica.
*/
class nkNukak3D: public wxFrame{
public:

	/** 
	 * Identificadores de los eventos.
	*/
	enum{
		ID_ABRIR_ARCHIVO = wxID_HIGHEST + 1500, /**< Abrir archivo. */
		ID_ABRIR_ARCHIVO_DICOM,			/**< Abrir pila de imagenes Dicom. */
		ID_ABRIR_ARCHIVO_MALLA3D,		/**< Abrir objeto 3D. */
		ID_ABRIR_ARCHIVO_VOL,			/**< Abrir archivo vol. */
		ID_ACERCA_DE,					/**< Mostrar Dialogo Acerca De. */
		ID_ACERCA_DE_MAC,				/**< Mostrar Dialodo Acerca De (Menu unicamente para Mac ) . */
		ID_ARBOL,						/**< Evento lanazado del nkToolBarMac. */
		ID_AREA,						/**< Calcular area de una region y mostrarla. */
		ID_CERRAR,						/**< Cerrar la aplicacion. */
		ID_CERRAR_TODO,					/**< Cerrar la apliacion. */
		ID_COLDET,						/**< Deteccion de colisiones. */
		ID_ENDOCAM,						/**< Habilitar el endoscopio virtual. */
		ID_FILPOLYDECIMATE,				/**< Reducir del numero poligonos. */
		ID_FILPOLYDEFORM,				/**< Deformar una malla. */
		ID_FILPOLYNORMALS,				/**< Recalcular las normales de una malla. */
		ID_FILPOLYSMOOTH,				/**< Suavizar una malla de poligonos. */
		ID_FILPOLYTRIANGLE,				/**< Triangular una malla de poligonos. */
		ID_FILVOLGAUSSIAN,				/**< Aplicar Filtro Gaussiana a una pila de imagenes. */
		ID_FILVOLGRADIENT,				/**< Aplicar Filtro Gradiente a una pila de imagenes. */
		ID_FILVOLMEDIAN,				/**< Aplicar Filtro Mediana a una pila de imagenes. */
		ID_FILVOLTHRESHOLD,				/**< Aplicar  Filtro umbral a una pila de imagenes. */
		ID_GUARDARMALLA3D,				/**< Guardar un objeto 3d. */
		ID_GUARDARVOL,					/**< Guardar una pila de imagenes en un archivo unico 3d. */
		ID_JOYSTICK,					/**< Dispositicvo de entrada joystick. */
		ID_LSLEVELSETSCOMPLETO,			/**< Ejecutar algoritmo LevelSets. */
		ID_MARCHING_CUBES,				/**< Ejecutar algoritmo Marching Cubes. */
		ID_NAVFLIGHT,					/**< Estilo de navegacion de la camara a estilo flotante. */
		ID_NAVJOYSTICK,					/**< Estilo de navegacion de la camara a Joystick. */
		ID_NAVRESET,					/**< Estilo de navegacion de la camara por defecto. */
		ID_NAVTRACKBALL,				/**< Estilo de navegacion de la camara a TrackBall. */
		ID_NAVUNICAM,					/**< Estilo de navegacion de la camara a estilo objeto. */
		ID_PALETA_COLOR,				/**< Estilo de navegacion de la camara a Joystick. */
		ID_PARIMAGE,					/**< Informacion de la pila de imagenes. */
		ID_PARPOLYGON,					/**< Informacion del objeto 3d. */
		ID_PARVIDEO,					/**< Informacion de la tarjeta de video. */
		ID_REINICIAR_PALETA,			/**< Reiniciar la paleta de colores. */
		ID_SALIR,						/**< Salir de la aplicacion. */
		ID_SNAPSHOT3D,					/**< Capturar la ventana 3D. */
		ID_SNAPSHOTAXIAL,				/**< Capturar la vista Axial. */
		ID_SNAPSHOTCORONAL,				/**< Capturar la vista Coronal. */
		ID_SNAPSHOTSAGITAL,				/**< Capturar la vista Sagital. */
		ID_STACTIVO,					/**< Activar visualizacion estero. */
		ID_STAUMENTAR,					/**< Aumentar separacion en visualizacion estereo. */
		ID_STDISMINUIR,					/**< Disminuir separacion en visualizacion estereo. */
		ID_STPASIVO,					/**< Activar desactivar visualizacion estereoscopica. */
		ID_VOLVIEWER_RENDERING_ESCALAR,	/**< Visualizacion del volumen con 3 planos . */
		ID_VOLVIEWER_RENDERING_MRC,		/**< Visualizacion del volumen con renderizacion por trazado de rayos . */
		ID_VOLVIEWER_RENDERING_TEXTURA,	/**< Visualizacion del volumen con renderizacion de textura. */
		ID_ULTIMO = wxID_HIGHEST+3000	/**< Eventos para los items de la paleta de colores. */
	};

	/**
	 * @brief Constructor de la clase.
	 * @details Ventana principal de Nukak3D.
	*/
	nkNukak3D(wxWindow* parent, int id=-1,
		const wxString& title=wxT(""),
        const wxPoint& pos=wxDefaultPosition,
        const wxSize& size=wxDefaultSize,
        long style=wxDEFAULT_FRAME_STYLE);
	/**
	 * @brief Destructor de la clase
	*/
	~nkNukak3D();

	/**
	 * @brief Insertar barra de herramientas lateral
	 * @param window nkToolBar a insertar
	 * @param un_nombre Nombre para identificar este toolbar
	 * @param una_etiqueta Etiqueta para mostrar en el panel que contiene este toolbar
	*/
	void insertarToolBar(wxWindow* window, wxString un_nombre, wxString una_etiqueta);

	/**
	 * @brief Abre una pila de imágenes) contenidas en un solo archivo
	*/
	void eventoAbrirVolumen(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Abre una pila de imagenes Dicom
	*/
	void eventoAbrirVolumenDicom(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Abre una malla poligonal 3D con formato vtk structured grid
	*/
	void eventoAbrirMalla3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Ventana "Acerca de"
	*/
	void eventoAcercaDe(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Ventana "Acerca de" para MAC unicamente
	*/
	void eventoAcercaDeMAC(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Para manejar los eventos de un nkToolbar en Mac o en sistemas Unix, no en Windows
	*/
	void eventoArbol(wxTreeEvent& event);

	/**
	 * @brief Guarda una pila de imágenes en un unico archivo en formato HDR
	*/
	void eventoGuardarVol(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Guarda una malla poligonal con formato vtk structured grid
	*/
	void eventoGuardarMalla3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Cambia la paleta de color(Look Up Table) de un volumen 3D para la visualizacion de una pila de imagenes
	*/
	void eventoPaletaColor(wxCommandEvent& event);

	/**
	 * @brief Reinicia la paleta de color(Look Up Table) de un volumen 3D con los valores predeterminados de la ventana (width and level)
	*/
	void eventoReiniciarPaleta(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Salir de la aplicación
	*/
	void eventoSalir(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D utilizando tres planos ortogonales
	*/
	void eventoVolViewerRenderingEscalar(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D usando precesamiento de textura.
	 * La opacidad se cambia variando la ventana (width and level) de la imagen
	*/
	void eventoVolViewerRenderingTextura(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D usando el algoritmo ray casting.
	 * La opacidad se cambia variando la ventana (width and level) de la imagen
	*/
	void eventoVolViewerRenderingMRC(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Genera isocontornos con el algoritmo Marching Cubes
	*/
	void eventoMarchingCubes(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Segmentación del volumen usando el algoritmo de levelsets
	*/
	void eventolsLevelsetsCompleto(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Calcula el área de la imagen axial sobre los pixeles con valor mayor que cero
	*/
	void eventoArea(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Resetear la cámara de la vista 3D
	*/
	void eventoNavResetCamara(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Navegación con el modo trackball. 
	 * @details Modo de navegación por defecto
	*/
	void eventoNavTrackball(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Navegación con el modo joystick
	*/
	void eventoNavJoystick(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Navegación con el modo flight
	*/
	void eventoNavFlight(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Navegación con el modo unicam
	*/
	void eventoNavUnicam(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Activar/desactivar visión estéreo activa
	*/
	void eventoStActivo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Activar/desactivar visión estéreo pasiva
	*/
	void eventoStPasivo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Aumentar separación entre ojos en el modo estéreo
	*/
	void eventoStAumentar(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Disminuir separación entre ojos en el modo estéreo
	*/
	void eventoStDisminuir(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Filtro gaussiano para un volumen 3D
	*/
	void eventoFilVolGaussian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Filtro mediana para un volumen 3D
	*/
	void eventoFilVolMedian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Filtro gradiente para un volumen 3D
	*/
	void eventoFilVolGradient(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Filtro umbral para un volumen 3D
	*/
	void eventoFilVolThreshold(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Procesar triangulado de una malla poligonal
	*/
	void eventoFilPolyTriangle(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Procesar decimado de una malla poligonal
	*/
	void eventoFilPolyDecimate(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Procesar suavizado de una malla poligonal
	*/
	void eventoFilPolySmooth(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Procesar normales de una malla poligonal
	*/
	void eventoFilPolyNormals(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Deformación de una malla poligonal
	*/
	void eventoFilPolyDeform(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Capturar pantalla vista 3D
	*/
	void eventoSnapshot3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Capturar pantalla vista axial
	*/
	void eventoSnapshotAxial(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Capturar pantalla vista sagital
	*/
	void eventoSnapshotSagital(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Capturar  pantalla vista coronal
	*/
	void eventoSnapshotCoronal(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Mostrar parámetros del volumen 3D
	*/
	void eventoParImage(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Mostrar parámetros de la malla poligonal
	*/
	void eventoParPolygon(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Mostrar parámetros de la tarjeta de video
	*/
	void eventoParVideo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Obtiene el notebook para que otra clase pueda crear otra página
	 * @return wxAuiNotebook *
	*/
	wxAuiNotebook * getNkNotebook(void);

private:
	wxAuiNotebook * prv_libro;		//! notebook de la interfaz gráfica	
	wxAuiManager prv_auiManager;	//! Administrador de la interfaz gráfica avanzada (aui)
	nkToolBar *  mi_nkHerramientas;	//! Barra de herramientas lateral Herramientas
	nkToolBar *  mi_nkImageViewer;	//! Barra de herramientas lateral Configuración
        
	// Joystick values
	wxJoystick* m_stick;			//! Joystick/Gamepad
    int     m_minX;					//! Valor máximo/mínimo del Joystick/Gamepad
    int     m_minY;					//! Valor máximo/mínimo del Joystick/Gamepad
	int     m_minZ;					//! Valor máximo/mínimo del Joystick/Gamepad
    int     m_maxX;					//! Valor máximo/mínimo del Joystick/Gamepad
    int     m_maxY;					//! Valor máximo/mínimo del Joystick/Gamepad
	int     m_maxZ;					//! Valor máximo/mínimo del Joystick/Gamepad
	int		nButtons;				//! Número de botones del Joystick/Gamepad
	int		nAxis;					//! Número de ejesdel Joystick/Gamepad
	unsigned int xpos;				//! Posición actual del Joystick/Gamepad
	unsigned int ypos;				//! Posición actual del Joystick/Gamepad

	DECLARE_EVENT_TABLE()			//! Declaración de la tabla de eventos
};


#endif //_NKNUKAK3D_H_