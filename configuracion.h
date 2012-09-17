#ifndef _CONFIGURACION_H
#define _CONFIGURACION_H

//Este archivo contiene toda la configuracion que debe proveerse para compilar el programa

/* ---------------------------------- Base de datos ----------------------------------------*/
#define DB_HOST "localhost"
#define DB_NAME "PANEL_DB_2"
#define DB_USER "root"
#define DB_PASS ""
/* -----------------------------------------------------------------------------------------*/

/* --------------------------- Funcionamiento del sistema ----------------------------------*/
#define SERVER_IP "216.245.212.74"
#define SERVER_IP2 "216.245.212.66"
//Este es el puerto default de la cuenta FTP Anonima (luego empieza a ascender de 2 en 2)
#define PUERTO_FTP_ANONIMA 2101
/* -----------------------------------------------------------------------------------------*/

/* ------------------------------- Directorios ---------------------------------------------*/
// Este es el directorio donde se encuentra el panel de administracion
#define PANEL_DIR "/www/docs/webs/phcm/"
/* -----------------------------------------------------------------------------------------*/

/* -------------------------------- Archivos -----------------------------------------------*/
/* Archivos de ruta completa ---------------------------------------------------------------*/
// Aqui se loguean todos los errores comunes para los paneles clientes
#define LOG_COMUN_FILE PANEL_DIR ".errores_panel"
/* Archivos de ruta completa ---------------------------------------------------------------*/
/* Archivos de ruta incompleta -------------------------------------------------------------*/
//Este archivo contiene la pagina de error para los errores fatales
#define ERROR_FATAL "FatalError.php"
// Estos archivos contienen las paginas de errores para los distintos comandos
#define ERROR_CAMBIAR_AUTORRESPUESTA "CambiarAutorrespuesta_Error.php"
#define ERROR_CAMBIAR_CLAVE_DB "CambiarClaveDB_Error.php"
#define ERROR_CAMBIAR_CLAVE_EXTENSION_FP "CambiarClaveExtensionFP_Error.php"
#define ERROR_CAMBIAR_CLAVE_FTP "CambiarClaveFTP_Error.php"
#define ERROR_CAMBIAR_CLAVE_FTP_ADMIN "CambiarClaveFTPAdmin_Error.php"
#define ERROR_CAMBIAR_CLAVE_POP3 "CambiarClavePOP3_Error.php"
#define ERROR_CAMBIAR_CLAVE_SHELL "CambiarClaveShell_Eror.php"
#define ERROR_CAMBIAR_CLAVE_USUARIO_ACCESO "CambiarClaveUsuarioAcceso_Error.php"
#define ERROR_CAMBIAR_MENSAJE_FTP_A "CambiarMensajeFTPA_Error.php"
#define ERROR_CREAR_ALIAS "CrearAlias_Error.php"
#define ERROR_CREAR_ALIAS_REDIR "CrearAliasRedir_Error.php"
#define ERROR_CREAR_AUTORRESPUESTA "CrearAutorrespuesta_Error.php"
#define ERROR_CREAR_BACKUP_DB "CrearBackupDB_Error.php"
#define ERROR_CREAR_BACKUP_EMAIL "CrearBackupEmail_Error.php"
#define ERROR_CREAR_BACKUP_FTP_A "CrearBackupFTPA_Error.php"
#define ERROR_CREAR_BACKUP_WEB "CrearBackupWeb_Error.php"
#define ERROR_CREAR_DOMINIO "CrearDominio_Error.php"
#define ERROR_CREAR_EXTENSION_FP "CrearExtensionFP_Error.php"
#define ERROR_CREAR_FTP "CrearFTP_Error.php"
#define ERROR_CREAR_FTP_A "CrearFTPAnonimo_Error.php"
#define ERROR_CREAR_MULTIDOMINIO "CrearMultidominio_Error.php"
#define ERROR_CREAR_POP3 "CrearPOP3_Error.php"
#define ERROR_CREAR_PAGINA_ERROR "CrearPaginaError_Error.php"
#define ERROR_CREAR_PROTECCION_ACCESO "CrearProteccionAcceso_Error.php"
#define ERROR_CREAR_PROTECCION_LISTADO "CrearProteccionListado_Error.php"
#define ERROR_CREAR_REDIRECCIONAMIENTO "CrearRedireccionamiento_Error.php"
#define ERROR_CREAR_REDIRECCIONAMIENTO_EXT "CrearRedireccionamientoExt_Error.php"
#define ERROR_CREAR_REDIRECCIONAMIENTO_SUB "CrearRedireccionamientoSub_Error.php"
#define ERROR_CREAR_SUBDOMINIO "CrearSubdominio_Error.php"
#define ERROR_CREAR_USUARIO_ACCESO "CrearUsuarioAcceso_Error.php"
#define ERROR_DESHABILITAR_ASP "DeshabilitarASP_Error.php"
#define ERROR_DESHABILITAR_CATCHALL "DeshabilitarCatchAll_Error.php"
#define ERROR_DESHABILITAR_DOMINIO "DeshabilitarDominio_Error.php"
#define ERROR_DESHABILITAR_FTP "DeshabilitarFTP_Error.php"
#define ERROR_DESHABILITAR_SHELL "DeshabilitarShell_Error.php"
#define ERROR_DESHABILITAR_USUARIO_ACCESO "DeshabilitarUsuarioAcceso_Error.php"
#define ERROR_ELIMINAR_ALIAS "EliminarAlias_Error.php"
#define ERROR_ELIMINAR_ALIAS_REDIR "EliminarAliasRedir_Error.php"
#define ERROR_ELIMINAR_AUTORRESPUESTA "EliminarAutorrespuesta_Error.php"
#define ERROR_ELIMINAR_DOMINIO "EliminarDominio_Error.php"
#define ERROR_ELIMINAR_EXTENSION_FP "EliminarExtensionFP_Error.php"
#define ERROR_ELIMINAR_FTP "EliminarFTP_Error.php"
#define ERROR_ELIMINAR_FTP_A "EliminarFTPAnonimo_Error.php"
#define ERROR_ELIMINAR_MULTIDOMINIO "EliminarMultidominio_Error.php"
#define ERROR_ELIMINAR_PAGINA_ERROR "EliminarPaginaError_Error.php"
#define ERROR_ELIMINAR_POP3 "EliminarPOP3_Error.php"
#define ERROR_ELIMINAR_PROTECCION_ACCESO "EliminarProteccionAcceso_Error.php"
#define ERROR_ELIMINAR_PROTECCION_LISTADO "EliminarProteccionListado_Error.php"
#define ERROR_ELIMINAR_REDIRECCIONAMIENTO "EliminarRedireccionamiento_Error.php"
#define ERROR_ELIMINAR_REDIRECCIONAMIENTO_EXT "EliminarRedireccionamientoExt_Error.php"
#define ERROR_ELIMINAR_REDIRECCIONAMIENTO_SUB "EliminarRedireccionamientoSub_Error.php"
#define ERROR_ELIMINAR_SUBDOMINIO "EliminarSubdominio_Error.php"
#define ERROR_ELIMINAR_USUARIO_ACCESO "EliminarUsuarioAcceso_Error.php"
#define ERROR_HABILITAR_ASP "HabilitarASP_Error.php"
#define ERROR_HABILITAR_CATCHALL "HabilitarCatchAll_Error.php"
#define ERROR_HABILITAR_DOMINIO "HabilitarDominio_Error.php"
#define ERROR_HABILITAR_FTP "HabilitarFTP_Error.php"
#define ERROR_HABILITAR_SHELL "HabilitarShell_Error.php"
#define ERROR_HABILITAR_USUARIO_ACCESO "HabilitarUsuarioAcceso_Error.php"
#define ERROR_VACIAR_DB "VaciarDB_Error.php"
// Estos archivos contienen las paginas de OK para los distintos comandos
#define OK_CAMBIAR_AUTORRESPUESTA "CambiarAutorrespuesta_OK.php"
#define OK_CAMBIAR_CLAVE_DB "CambiarClaveDB_OK.php"
#define OK_CAMBIAR_CLAVE_EXTENSION_FP "CambiarClaveExtensionFP_OK.php"
#define OK_CAMBIAR_CLAVE_FTP "CambiarClaveFTP_OK.php"
#define OK_CAMBIAR_CLAVE_FTP_ADMIN "CambiarClaveFTPAdmin_OK.php"
#define OK_CAMBIAR_CLAVE_POP3 "CambiarClavePOP3_OK.php"
#define OK_CAMBIAR_CLAVE_SHELL "CambiarClaveShell_OK.php"
#define OK_CAMBIAR_CLAVE_USUARIO_ACCESO "CambiarClaveUsuarioAcceso_OK.php"
#define OK_CAMBIAR_MENSAJE_FTP_A "CambiarMensajeFTPA_OK.php"
#define OK_CREAR_ALIAS "CrearAlias_OK.php"
#define OK_CREAR_ALIAS_REDIR "CrearAliasRedir_OK.php"
#define OK_CREAR_AUTORRESPUESTA "CrearAutorrespuesta_OK.php"
#define OK_CREAR_BACKUP_DB "CrearBackupDB_OK.php"
#define OK_CREAR_BACKUP_EMAIL "CrearBackupEmail_OK.php"
#define OK_CREAR_BACKUP_FTP_A "CrearBackupFTPA_OK.php"
#define OK_CREAR_BACKUP_WEB "CrearBackupWeb_OK.php"
#define OK_CREAR_DOMINIO "CrearDominio_OK.php"
#define OK_CREAR_EXTENSION_FP "CrearExtensionFP_OK.php"
#define OK_CREAR_FTP "CrearFTP_OK.php"
#define OK_CREAR_FTP_A "CrearFTPAnonimo_OK.php"
#define OK_CREAR_MULTIDOMINIO "CrearMultidominio_OK.php"
#define OK_CREAR_POP3 "CrearPOP3_OK.php"
#define OK_CREAR_PAGINA_ERROR "CrearPaginaError_OK.php"
#define OK_CREAR_PROTECCION_ACCESO "CrearProteccionAcceso_OK.php"
#define OK_CREAR_PROTECCION_LISTADO "CrearProteccionListado_OK.php"
#define OK_CREAR_REDIRECCIONAMIENTO "CrearRedireccionamiento_OK.php"
#define OK_CREAR_REDIRECCIONAMIENTO_EXT "CrearRedireccionamientoExt_OK.php"
#define OK_CREAR_REDIRECCIONAMIENTO_SUB "CrearRedireccionamientoSub_OK.php"
#define OK_CREAR_SUBDOMINIO "CrearSubdominio_OK.php"
#define OK_CREAR_USUARIO_ACCESO "CrearUsuarioAcceso_OK.php"
#define OK_DESHABILITAR_ASP "DeshabilitarASP_OK.php"
#define OK_DESHABILITAR_DOMINIO "DeshabilitarDominio_OK.php"
#define OK_DESHABILITAR_FTP "DeshabilitarFTP_OK.php"
#define OK_DESHABILITAR_SHELL "DeshabilitarShell_OK.php"
#define OK_DESHABILITAR_USUARIO_ACCESO "DeshabilitarUsuarioAcceso_OK.php"
#define OK_ELIMINAR_ALIAS "EliminarAlias_OK.php"
#define OK_ELIMINAR_ALIAS_REDIR "EliminarAliasRedir_OK.php"
#define OK_ELIMINAR_AUTORRESPUESTA "EliminarAutorrespuesta_OK.php"
#define OK_ELIMINAR_DOMINIO "EliminarDominio_OK.php"
#define OK_ELIMINAR_EXTENSION_FP "EliminarExtensionFP_OK.php"
#define OK_ELIMINAR_FTP "EliminarFTP_OK.php"
#define OK_ELIMINAR_FTP_A "EliminarFTPAnonimo_OK.php"
#define OK_ELIMINAR_MULTIDOMINIO "EliminarMultidominio_OK.php"
#define OK_ELIMINAR_PAGINA_ERROR "EliminarPaginaError_OK.php"
#define OK_ELIMINAR_POP3 "EliminarPOP3_OK.php"
#define OK_ELIMINAR_PROTECCION_ACCESO "EliminarProteccionAcceso_OK.php"
#define OK_ELIMINAR_PROTECCION_LISTADO "EliminarProteccionListado_OK.php"
#define OK_ELIMINAR_REDIRECCIONAMIENTO "EliminarRedireccionamiento_OK.php"
#define OK_ELIMINAR_REDIRECCIONAMIENTO_EXT "EliminarRedireccionamientoExt_OK.php"
#define OK_ELIMINAR_REDIRECCIONAMIENTO_SUB "EliminarRedireccionamientoSub_OK.php"
#define OK_ELIMINAR_SUBDOMINIO "EliminarSubdominio_OK.php"
#define OK_ELIMINAR_USUARIO_ACCESO "EliminarUsuarioAcceso_OK.php"
#define OK_HABILITAR_ASP "HabilitarASP_OK.php"
#define OK_HABILITAR_DOMINIO "HabilitarDominio_OK.php"
#define OK_HABILITAR_FTP "HabilitarFTP_OK.php"
#define OK_HABILITAR_SHELL "HabilitarShell_OK.php"
#define OK_HABILITAR_USUARIO_ACCESO "HabilitarUsuarioAcceso_OK.php"
#define OK_VACIAR_DB "VaciarDB_OK.php"
/* Archivos de ruta incompleta -------------------------------------------------------------*/
/* -----------------------------------------------------------------------------------------*/

/* ----------------------------- Variables Miscelaneas -------------------------------------*/
#define MAXLINE 8192
#define MINLINE 256
/* -----------------------------------------------------------------------------------------*/

#endif
