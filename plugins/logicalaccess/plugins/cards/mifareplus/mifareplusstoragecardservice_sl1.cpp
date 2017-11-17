//
// Created by xaqq on 7/2/15.
//

#include <assert.h>
#include <logicalaccess/plugins/cards/mifareplus/mifareplusstoragecardservice_sl1.hpp>
#include <logicalaccess/plugins/cards/mifareplus/mifareplusaccessinfo_sl1.hpp>
#include <logicalaccess/plugins/cards/mifareplus/mifareplussl1commands.hpp>
#include <logicalaccess/logs.hpp>


logicalaccess::MifarePlusStorageSL1CardService::MifarePlusStorageSL1CardService(
    std::shared_ptr<Chip> chip)
    : MifareStorageCardService(chip)
    , has_been_authenticated_(false)
{
}

void logicalaccess::MifarePlusStorageSL1CardService::erase(
    std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse)
{
    authenticate_if_needed(aiToUse);
    MifareStorageCardService::erase(location, aiToUse);
}

void logicalaccess::MifarePlusStorageSL1CardService::writeData(
    std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse,
    std::shared_ptr<AccessInfo> aiToWrite, const ByteVector &data,
    CardBehavior behaviorFlags)
{
    authenticate_if_needed(aiToUse);
    MifareStorageCardService::writeData(location, aiToUse, aiToWrite, data,
                                        behaviorFlags);
}

ByteVector logicalaccess::MifarePlusStorageSL1CardService::readData(
    std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse,
    size_t length, CardBehavior behaviorFlags)
{
    authenticate_if_needed(aiToUse);
    return MifareStorageCardService::readData(location, aiToUse, length, behaviorFlags);
}

ByteVector logicalaccess::MifarePlusStorageSL1CardService::readDataHeader(
    std::shared_ptr<Location> location, std::shared_ptr<AccessInfo> aiToUse)
{
    authenticate_if_needed(aiToUse);
    return MifareStorageCardService::readDataHeader(location, aiToUse);
}

void logicalaccess::MifarePlusStorageSL1CardService::authenticate_if_needed(
    std::shared_ptr<AccessInfo> ai)
{
    auto mfp_sl1_ai = std::dynamic_pointer_cast<MifarePlusSL1AccessInfo>(ai);
    auto mfp_sl1_cmd =
        std::dynamic_pointer_cast<MifarePlusSL1Commands>(getChip()->getCommands());

    // if the access info is not specific to MFP SL1, simply do not
    // proceed to AES auth.
    if (!mfp_sl1_ai)
    {
        return;
    }
    assert(mfp_sl1_ai->aesAuthKey);
    assert(mfp_sl1_cmd);

    if (!mfp_sl1_ai->aesAuthKey->isEmpty() && !has_been_authenticated_)
    {
        if (mfp_sl1_cmd->AESAuthenticateSL1(mfp_sl1_ai->aesAuthKey))
            has_been_authenticated_ = true;
    }
}