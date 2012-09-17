SHELL = /bin/sh

CC=ccache g++
CFLAGS = -Wall -o2 -I/usr/local/include -I./ -L/usr/local/mysql/lib/mysql/lib -L/usr/local/lib -L./ -I/usr/local/mysql/include/mysql -Wno-deprecated
MYSQLLIB = -lmysqlpp
CGICC = -lcgicc

OBJS = dominio.o log.o servicio.o interfaz.o util.o tiempo.o
OBJS_Dominio = servicioDominio.o
OBJS_POP3 = servicioPOP3.o
OBJS_CrearPOP3 = $(OBJS) $(OBJS_POP3) $(OBJS_Alias) CrearPOP3.o
OBJS_EliminarPOP3 = $(OBJS) $(OBJS_POP3) $(OBJS_Autorrespuesta) EliminarPOP3.o
OBJS_CambiarClavePOP3 = $(OBJS) $(OBJS_POP3) CambiarClavePOP3.o
OBJS_Alias = servicioAlias.o
OBJS_CrearAlias = $(OBJS) $(OBJS_Alias) $(OBJS_POP3) CrearAlias.o
OBJS_EliminarAlias = $(OBJS) $(OBJS_Alias) EliminarAlias.o
OBJS_AliasRedir = servicioAliasRedir.o
OBJS_CrearAliasRedir = $(OBJS) $(OBJS_AliasRedir) $(OBJS_POP3) CrearAliasRedir.o
OBJS_EliminarAliasRedir = $(OBJS) $(OBJS_AliasRedir) EliminarAliasRedir.o
OBJS_Autorrespuesta = servicioAutorrespuesta.o
OBJS_CrearAutorrespuesta = $(OBJS) $(OBJS_Autorrespuesta) $(OBJS_POP3) CrearAutorrespuesta.o
OBJS_EliminarAutorrespuesta = $(OBJS) $(OBJS_Autorrespuesta) EliminarAutorrespuesta.o
OBJS_CambiarAutorrespuesta = $(OBJS) $(OBJS_Autorrespuesta) CambiarAutorrespuesta.o
OBJS_FTPAdmin = servicioFTPAdmin.o
OBJS_CambiarClaveFTPAdmin = $(OBJS) $(OBJS_FTPAdmin) CambiarClaveFTPAdmin.o
OBJS_DB = servicioDB.o
OBJS_CambiarClaveDB = $(OBJS) $(OBJS_DB) $(OBJS_ASP) CambiarClaveDB.o
OBJS_VaciarDB = $(OBJS) $(OBJS_DB) VaciarDB.o
OBJS_Subdominio = servicioSubdominio.o
OBJS_CrearSubdominio = $(OBJS) $(OBJS_Subdominio) $(OBJS_FTP) $(OBJS_ProteccionAcceso) $(OBJS_ProteccionListado) CrearSubdominio.o
OBJS_EliminarSubdominio = $(OBJS) $(OBJS_Subdominio) $(OBJS_ProteccionAcceso) $(OBJS_ProteccionListado) EliminarSubdominio.o
OBJS_FTP = servicioFTP.o
OBJS_CrearFTP = $(OBJS) $(OBJS_FTP) $(OBJS_Subdominio) $(OBJS_ProteccionAcceso) $(OBJS_ProteccionListado) CrearFTP.o
OBJS_EliminarFTP = $(OBJS) $(OBJS_FTP) $(OBJS_ProteccionAcceso) $(OBJS_ProteccionListado) EliminarFTP.o
OBJS_CambiarClaveFTP = $(OBJS) $(OBJS_FTP) CambiarClaveFTP.o
OBJS_HabilitarFTP = $(OBJS) $(OBJS_FTP) HabilitarFTP.o
OBJS_DeshabilitarFTP = $(OBJS) $(OBJS_FTP) DeshabilitarFTP.o
OBJS_ASP = servicioASP.o
OBJS_HabilitarASP = $(OBJS) $(OBJS_ASP) $(OBJS_DB) HabilitarASP.o
OBJS_DeshabilitarASP = $(OBJS) $(OBJS_ASP) $(OBJS_DB) DeshabilitarASP.o
OBJS_FTPAnonimo = servicioFTPAnonimo.o
OBJS_CrearFTPAnonimo = $(OBJS) $(OBJS_FTPAnonimo) CrearFTPAnonimo.o
OBJS_EliminarFTPAnonimo = $(OBJS) $(OBJS_FTPAnonimo) EliminarFTPAnonimo.o
OBJS_CambiarMensajeFTPA = $(OBJS) $(OBJS_FTPAnonimo) CambiarMensajeFTPA.o
OBJS_Redireccionamiento = servicioRedireccionamiento.o
OBJS_CrearRedireccionamiento = $(OBJS) $(OBJS_Redireccionamiento) $(OBJS_Dominio) $(OBJS_RedireccionamientoExt) $(OBJS_RedireccionamientoSub) $(OBJS_Multidominio) CrearRedireccionamiento.o
OBJS_EliminarRedireccionamiento = $(OBJS) $(OBJS_Redireccionamiento) EliminarRedireccionamiento.o
OBJS_RedireccionamientoSub = servicioRedireccionamientoSub.o
OBJS_CrearRedireccionamientoSub = $(OBJS) $(OBJS_RedireccionamientoSub) $(OBJS_Dominio) $(OBJS_Redireccionamiento) $(OBJS_RedireccionamientoExt) $(OBJS_Multidominio) CrearRedireccionamientoSub.o
OBJS_EliminarRedireccionamientoSub = $(OBJS) $(OBJS_RedireccionamientoSub) EliminarRedireccionamientoSub.o
OBJS_RedireccionamientoExt = servicioRedireccionamientoExt.o
OBJS_CrearRedireccionamientoExt = $(OBJS) $(OBJS_RedireccionamientoExt) $(OBJS_Dominio) $(OBJS_Redireccionamiento) $(OBJS_RedireccionamientoSub) $(OBJS_Multidominio) CrearRedireccionamientoExt.o
OBJS_EliminarRedireccionamientoExt = $(OBJS) $(OBJS_RedireccionamientoExt) EliminarRedireccionamientoExt.o
OBJS_Multidominio = servicioMultidominio.o
OBJS_CrearMultidominio = $(OBJS) $(OBJS_Multidominio) $(OBJS_Dominio) $(OBJS_Redireccionamiento) $(OBJS_RedireccionamientoExt) $(OBJS_RedireccionamientoSub) CrearMultidominio.o
OBJS_EliminarMultidominio = $(OBJS) $(OBJS_Multidominio) EliminarMultidominio.o
OBJS_Shell = servicioShell.o
OBJS_HabilitarShell = $(OBJS) $(OBJS_Shell) HabilitarShell.o
OBJS_DeshabilitarShell = $(OBJS) $(OBJS_Shell) DeshabilitarShell.o
OBJS_CambiarClaveShell = $(OBJS) $(OBJS_Shell) CambiarClaveShell.o
OBJS_ExtensionFP = servicioExtensionFP.o
OBJS_CrearExtensionFP = $(OBJS) $(OBJS_ExtensionFP) CrearExtensionFP.o
OBJS_EliminarExtensionFP = $(OBJS) $(OBJS_ExtensionFP) EliminarExtensionFP.o
OBJS_CambiarClaveExtensionFP = $(OBJS) $(OBJS_ExtensionFP) CambiarClaveExtensionFP.o
OBJS_PaginaError = servicioPaginaError.o
OBJS_CrearPaginaError = $(OBJS) $(OBJS_PaginaError) CrearPaginaError.o
OBJS_EliminarPaginaError = $(OBJS) $(OBJS_PaginaError) EliminarPaginaError.o
OBJS_ProteccionListado = servicioProteccionListado.o
OBJS_CrearProteccionListado = $(OBJS) $(OBJS_ProteccionListado) $(OBJS_ProteccionAcceso) $(OBJS_Subdominio) $(OBJS_FTP) CrearProteccionListado.o
OBJS_EliminarProteccionListado = $(OBJS) $(OBJS_ProteccionListado) $(OBJS_Subdominio) $(OBJS_FTP) EliminarProteccionListado.o
OBJS_Backup = servicioBackup.o
OBJS_CrearBackupDB = $(OBJS) $(OBJS_Backup) CrearBackupDB.o
OBJS_CrearBackupEmail = $(OBJS) $(OBJS_Backup) $(OBJS_POP3) CrearBackupEmail.o
OBJS_CrearBackupFTPA = $(OBJS) $(OBJS_Backup) CrearBackupFTPA.o
OBJS_CrearBackupWeb = $(OBJS) $(OBJS_Backup) CrearBackupWeb.o
OBJS_ProteccionAcceso = servicioProteccionAcceso.o
OBJS_CrearProteccionAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_ProteccionListado) $(OBJS_Subdominio) $(OBJS_FTP) CrearProteccionAcceso.o
OBJS_EliminarProteccionAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_Subdominio) $(OBJS_FTP) EliminarProteccionAcceso.o
OBJS_UsuarioAcceso = servicioUsuarioAcceso.o
OBJS_CrearUsuarioAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_UsuarioAcceso) CrearUsuarioAcceso.o
OBJS_EliminarUsuarioAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_UsuarioAcceso) EliminarUsuarioAcceso.o
OBJS_CambiarClaveUsuarioAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_UsuarioAcceso) CambiarClaveUsuarioAcceso.o
OBJS_HabilitarUsuarioAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_UsuarioAcceso) HabilitarUsuarioAcceso.o
OBJS_DeshabilitarUsuarioAcceso = $(OBJS) $(OBJS_ProteccionAcceso) $(OBJS_UsuarioAcceso) DeshabilitarUsuarioAcceso.o
OBJS_WebAlizer= log.o WebAlizer.o util.o
OBJS_Http-A= log.o Http-A.o util.o

BINS = CrearPOP3.cgi EliminarPOP3.cgi CambiarClavePOP3.cgi CrearAlias.cgi CrearAliasRedir.cgi EliminarAlias.cgi EliminarAliasRedir.cgi CrearAutorrespuesta.cgi EliminarAutorrespuesta.cgi CambiarAutorrespuesta.cgi CambiarClaveFTPAdmin.cgi CambiarClaveDB.cgi CrearSubdominio.cgi EliminarSubdominio.cgi CrearFTP.cgi EliminarFTP.cgi CambiarClaveFTP.cgi HabilitarFTP.cgi DeshabilitarFTP.cgi HabilitarASP.cgi DeshabilitarASP.cgi CrearFTPAnonimo.cgi EliminarFTPAnonimo.cgi CambiarMensajeFTPA.cgi CrearRedireccionamiento.cgi EliminarRedireccionamiento.cgi CrearRedireccionamientoSub.cgi EliminarRedireccionamientoSub.cgi CrearRedireccionamientoExt.cgi EliminarRedireccionamientoExt.cgi CrearMultidominio.cgi EliminarMultidominio.cgi HabilitarShell.cgi DeshabilitarShell.cgi CambiarClaveShell.cgi CrearExtensionFP.cgi EliminarExtensionFP.cgi CambiarClaveExtensionFP.cgi CrearPaginaError.cgi EliminarPaginaError.cgi CrearProteccionListado.cgi EliminarProteccionListado.cgi CrearBackupDB.cgi CrearBackupEmail.cgi CrearBackupFTPA.cgi CrearBackupWeb.cgi CrearProteccionAcceso.cgi EliminarProteccionAcceso.cgi VaciarDB.cgi CrearUsuarioAcceso.cgi EliminarUsuarioAcceso.cgi HabilitarUsuarioAcceso.cgi DeshabilitarUsuarioAcceso.cgi CambiarClaveUsuarioAcceso.cgi WebAlizer Http-A

all: $(BINS)

CrearPOP3.cgi: $(OBJS_CrearPOP3)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarPOP3.cgi: $(OBJS_EliminarPOP3)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClavePOP3.cgi: $(OBJS_CambiarClavePOP3)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearAlias.cgi: $(OBJS_CrearAlias)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarAlias.cgi: $(OBJS_EliminarAlias)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearAliasRedir.cgi: $(OBJS_CrearAliasRedir)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarAliasRedir.cgi: $(OBJS_EliminarAliasRedir)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^	

CrearAutorrespuesta.cgi: $(OBJS_CrearAutorrespuesta)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarAutorrespuesta.cgi: $(OBJS_EliminarAutorrespuesta)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarAutorrespuesta.cgi: $(OBJS_CambiarAutorrespuesta)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveFTPAdmin.cgi: $(OBJS_CambiarClaveFTPAdmin)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveDB.cgi: $(OBJS_CambiarClaveDB)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

VaciarDB.cgi: $(OBJS_VaciarDB)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearSubdominio.cgi: $(OBJS_CrearSubdominio)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarSubdominio.cgi: $(OBJS_EliminarSubdominio)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearFTP.cgi: $(OBJS_CrearFTP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarFTP.cgi: $(OBJS_EliminarFTP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveFTP.cgi: $(OBJS_CambiarClaveFTP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

HabilitarFTP.cgi: $(OBJS_HabilitarFTP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

DeshabilitarFTP.cgi: $(OBJS_DeshabilitarFTP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

HabilitarASP.cgi: $(OBJS_HabilitarASP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

DeshabilitarASP.cgi: $(OBJS_DeshabilitarASP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearFTPAnonimo.cgi: $(OBJS_CrearFTPAnonimo)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarFTPAnonimo.cgi: $(OBJS_EliminarFTPAnonimo)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarMensajeFTPA.cgi: $(OBJS_CambiarMensajeFTPA)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearRedireccionamiento.cgi: $(OBJS_CrearRedireccionamiento)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarRedireccionamiento.cgi: $(OBJS_EliminarRedireccionamiento)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearRedireccionamientoSub.cgi: $(OBJS_CrearRedireccionamientoSub)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarRedireccionamientoSub.cgi: $(OBJS_EliminarRedireccionamientoSub)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearRedireccionamientoExt.cgi: $(OBJS_CrearRedireccionamientoExt)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarRedireccionamientoExt.cgi: $(OBJS_EliminarRedireccionamientoExt)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearMultidominio.cgi: $(OBJS_CrearMultidominio)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarMultidominio.cgi: $(OBJS_EliminarMultidominio)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

HabilitarShell.cgi: $(OBJS_HabilitarShell)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

DeshabilitarShell.cgi: $(OBJS_DeshabilitarShell)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveShell.cgi: $(OBJS_CambiarClaveShell)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearExtensionFP.cgi: $(OBJS_CrearExtensionFP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarExtensionFP.cgi: $(OBJS_EliminarExtensionFP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveExtensionFP.cgi: $(OBJS_CambiarClaveExtensionFP)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearPaginaError.cgi: $(OBJS_CrearPaginaError)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarPaginaError.cgi: $(OBJS_EliminarPaginaError)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearProteccionListado.cgi: $(OBJS_CrearProteccionListado)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarProteccionListado.cgi: $(OBJS_EliminarProteccionListado)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearBackupDB.cgi: $(OBJS_CrearBackupDB)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearBackupEmail.cgi: $(OBJS_CrearBackupEmail)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearBackupFTPA.cgi: $(OBJS_CrearBackupFTPA)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearBackupWeb.cgi: $(OBJS_CrearBackupWeb)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearProteccionAcceso.cgi: $(OBJS_CrearProteccionAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarProteccionAcceso.cgi: $(OBJS_EliminarProteccionAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CrearUsuarioAcceso.cgi: $(OBJS_CrearUsuarioAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

EliminarUsuarioAcceso.cgi: $(OBJS_EliminarUsuarioAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

HabilitarUsuarioAcceso.cgi: $(OBJS_HabilitarUsuarioAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

DeshabilitarUsuarioAcceso.cgi: $(OBJS_DeshabilitarUsuarioAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

CambiarClaveUsuarioAcceso.cgi: $(OBJS_CambiarClaveUsuarioAcceso)
	$(CC) $(CFLAGS) $(MYSQLLIB) $(CGICC) -o $@ $^

WebAlizer: $(OBJS_WebAlizer)
	$(CC) $(CFLAGS) $(MYSQLLIB) -o $@ $^

Http-A: $(OBJS_Http-A)
	$(CC) $(CFLAGS) $(MYSQLLIB) -o $@ $^	

%.o: %.cpp *.h
	$(CC) $(CFLAGS) -c -o $*.o $<

.PHONY: clean install unistall

install:
	cp $(BINS) /www/docs/panelphcm/
	chmod u+s /www/docs/panelphcm/*.cgi

uninstall:
	rm /www/docs/webs/verga.com/CrearPOP3.cgi
	rm /www/docs/webs/verga.com/EliminarPOP3.cgi
	rm /www/docs/webs/verga.com/CambiarClavePOP3.cgi
	rm /www/docs/webs/verga.com/CrearAlias.cgi
	rm /www/docs/webs/verga.com/EliminarAlias.cgi
	rm /www/docs/webs/verga.com/CrearAutorrespuesta.cgi
	rm /www/docs/webs/verga.com/EliminarAutorrespuesta.cgi
	rm /www/docs/webs/verga.com/CambiarAutorrespuesta.cgi
	rm /www/docs/webs/verga.com/CambiarClaveFTPAdmin.cgi
	rm /www/docs/webs/verga.com/CambiarClaveDB.cgi
	rm /www/docs/webs/verga.com/VaciarDB.cgi
	rm /www/docs/webs/verga.com/CrearSubdominio.cgi
	rm /www/docs/webs/verga.com/EliminarSubdominio.cgi
	rm /www/docs/webs/verga.com/CrearFTP.cgi
	rm /www/docs/webs/verga.com/EliminarFTP.cgi
	rm /www/docs/webs/verga.com/CambiarClaveFTP.cgi
	rm /www/docs/webs/verga.com/HabilitarFTP.cgi
	rm /www/docs/webs/verga.com/DeshabilitarFTP.cgi
	rm /www/docs/webs/verga.com/HabilitarASP.cgi
	rm /www/docs/webs/verga.com/DeshabilitarASP.cgi
	rm /www/docs/webs/verga.com/CrearFTPAnonimo.cgi
	rm /www/docs/webs/verga.com/EliminarFTPAnonimo.cgi
	rm /www/docs/webs/verga.com/CambiarMensajeFTPA.cgi
	rm /www/docs/webs/verga.com/CrearRedireccionamiento.cgi
	rm /www/docs/webs/verga.com/EliminarRedireccionamiento.cgi
	rm /www/docs/webs/verga.com/CrearRedireccionamientoSub.cgi
	rm /www/docs/webs/verga.com/EliminarRedireccionamientoSub.cgi
	rm /www/docs/webs/verga.com/CrearRedireccionamientoExt.cgi
	rm /www/docs/webs/verga.com/EliminarRedireccionamientoExt.cgi
	rm /www/docs/webs/verga.com/CrearMultidominio.cgi
	rm /www/docs/webs/verga.com/EliminarMultidominio.cgi
	rm /www/docs/webs/verga.com/HabilitarShell.cgi
	rm /www/docs/webs/verga.com/DeshabilitarShell.cgi
	rm /www/docs/webs/verga.com/CambiarClaveShell.cgi
	rm /www/docs/webs/verga.com/CrearExtensionFP.cgi
	rm /www/docs/webs/verga.com/EliminarExtensionFP.cgi
	rm /www/docs/webs/verga.com/CambiarClaveExtensionFP.cgi
	rm /www/docs/webs/verga.com/CrearPaginaError.cgi
	rm /www/docs/webs/verga.com/EliminarPaginaError.cgi
	rm /www/docs/webs/verga.com/CrearProteccionListado.cgi
	rm /www/docs/webs/verga.com/EliminarProteccionListado.cgi
	rm /www/docs/webs/verga.com/BackupDB.cgi
	rm /www/docs/webs/verga.com/BackupFTPA.cgi
	rm /www/docs/webs/verga.com/BackupMail.cgi
	rm /www/docs/webs/verga.com/BackupWeb.cgi
	rm /www/docs/webs/verga.com/CrearProteccionAcceso.cgi
	rm /www/docs/webs/verga.com/EliminarProteccionAcceso.cgi
	rm /www/docs/webs/verga.com/CrearUsuarioAcceso.cgi
	rm /www/docs/webs/verga.com/EliminarUsuarioAcceso.cgi
	rm /www/docs/webs/verga.com/CambiarClaveUsuarioAcceso.cgi
	rm /www/docs/webs/verga.com/HabilitarUsuarioAcceso.cgi
	rm /www/docs/webs/verga.com/DeshabilitarUsuarioAcceso.cgi

clean:
	$(RM) $(BINS) *.o *~ *.*~

