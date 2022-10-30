// matth-x/ArduinoOcpp
// Copyright Matthias Akstaller 2019 - 2022
// MIT License

#ifndef TRANSACTIONSTORE_H
#define TRANSACTIONSTORE_H

#include <ArduinoOcpp/Tasks/Transactions/Transaction.h>
#include <ArduinoOcpp/Core/Configuration.h>
#include <ArduinoOcpp/Core/FilesystemAdapter.h>
#include <deque>

namespace ArduinoOcpp {

class TransactionStore;

class ConnectorTransactionStore {
private:
    TransactionStore& context;
    const uint connectorId;

    std::shared_ptr<FilesystemAdapter> filesystem;
    std::shared_ptr<Configuration<int>> txEnd;
    
    std::deque<std::weak_ptr<Transaction>> transactions;

public:
    ConnectorTransactionStore(TransactionStore& context, uint connectorId, std::shared_ptr<FilesystemAdapter> filesystem);
    
    std::shared_ptr<Transaction> getLatestTransaction();
    bool commit(Transaction *transaction);

    std::shared_ptr<Transaction> getTransaction(unsigned int txNr);
    std::shared_ptr<Transaction> createTransaction();
};

class TransactionStore {
private:
    std::vector<std::unique_ptr<ConnectorTransactionStore>> connectors;
public:
    TransactionStore(uint nConnectors, std::shared_ptr<FilesystemAdapter> filesystem);

    std::shared_ptr<Transaction> getLatestTransaction(uint connectorId);
    bool commit(Transaction *transaction);

    std::shared_ptr<Transaction> getTransaction(unsigned int connectorId, unsigned int txNr);
    std::shared_ptr<Transaction> createTransaction(unsigned int connectorId);
};

}

#endif
