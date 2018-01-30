#include "connector.h"


void Connector::Connect(const ConnectionConfig config)
{
	//Поиск уже созданного соединения
	auto searchableConnection = std::find_if(connections.begin(), connections.end(), [&config](const std::shared_ptr<Connection>& connection)
	{
		return connection->GetPortConfig().portName == config.portConfig.portName;
	});
	
	if (searchableConnection != connections.end())
	{
		//Добавляем в уже существующее соединение
		(*searchableConnection)->AddController(config.controllerConfig);
	}
	else
	{
		//Создаем новое соединение
		auto newConnection = std::make_shared<Connection>();
		newConnection->AddController(config.controllerConfig);
		newConnection->Open(config.portConfig);
		auto portConfig = newConnection->GetPortConfig();
		emit PortStatusChange(portConfig.portName, newConnection->GetPortStatus());
		QObject::connect(newConnection.get(), &Connection::PortStatusChange, [&, portConfig](Connection::PortStatus status)
		{
			emit PortStatusChange(portConfig.portName, status);
		});
		connections.push_back(newConnection);
	}
}


void Connector::Disconnect(const QString & controllerName)
{
	for (auto connection : connections)
	{
		auto controllersConfig = connection->GetControllersConfig();
		for (auto& controllerConfig : controllersConfig)
		{
			if (controllerConfig.controller->GetName() == controllerName)
			{
				connection->RemoveController(controllerName);
			}
		}
		controllersConfig = connection->GetControllersConfig();
		if (controllersConfig.size() == 0)
		{
			connections.removeAll(connection);
		}
	}
}


Connector::ConnectionConfig Connector::GetConfigController(const QString& name)
{
	for (auto& connection : connections)
	{
		auto controllersConfig = connection->GetControllersConfig();
		for (auto& controllerConfig : controllersConfig)
		{
			if (controllerConfig.controller->GetName() == name)
			{
				ConnectionConfig connectionConfig;
				connectionConfig.controllerConfig = controllerConfig;
				connectionConfig.portConfig = connection->GetPortConfig();
				return connectionConfig;
			}
		}
	}
	return ConnectionConfig();
}

void Connector::DisconnectAll(void)
{
	for (auto& connection : connections)
	{
		connection->Close();
	}
}
