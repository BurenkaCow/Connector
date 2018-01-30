#pragma once
#include <qObject>
#include <memory>
#include "modbus/connection.h"


//Диспетчер соединений
class Connector : public QObject
{
	Q_OBJECT
public:
	struct ConnectionConfig //Конфигурация соединения
	{
		Connection::ConfigSerialPort portConfig; //Конфигурация порта
		Connection::ControllerConfig controllerConfig; //Конфигурация контроллера
	};
public:
	Connector(void) {}
	void Connect(const ConnectionConfig config); //Подключить контроллер
	void Disconnect(const QString& controllerName); //Отключить контроллер
	void DisconnectAll(void); //Завершить все соединения
	ConnectionConfig GetConfigController(const QString& name); //Вернуть конфигурацию соединения нужного контроллера
	~Connector(void) {}
signals:
	void PortStatusChange(QString name, Connection::PortStatus status); //Статус порта изменился
	void ControllerStatusChange(QString name, Controller::Status status); //Статус контроллера изменился
private:
	QList<std::shared_ptr<Connection>> connections; //Список соединений
};

