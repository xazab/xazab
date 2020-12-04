// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2020 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <arith_uint256.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateDevNetGenesisBlock(const uint256 &prevBlockHash, const std::string& devNetName, uint32_t nTime, uint32_t nNonce, uint32_t nBits, const CAmount& genesisReward)
{
    assert(!devNetName.empty());

    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    // put height (BIP34) and devnet name into coinbase
    txNew.vin[0].scriptSig = CScript() << 1 << std::vector<unsigned char>(devNetName.begin(), devNetName.end());
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_RETURN;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = 4;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock = prevBlockHash;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Wired 09/Jan/2014 The Grand Experiment Goes Live: Overstock.com Is Now Accepting Bitcoins";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout, int64_t nWindowSize, int64_t nThresholdStart, int64_t nThresholdMin, int64_t nFalloffCoeff)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
    if (nWindowSize != -1) {
            consensus.vDeployments[d].nWindowSize = nWindowSize;
    }
    if (nThresholdStart != -1) {
        consensus.vDeployments[d].nThresholdStart = nThresholdStart;
    }
    if (nThresholdMin != -1) {
        consensus.vDeployments[d].nThresholdMin = nThresholdMin;
    }
    if (nFalloffCoeff != -1) {
        consensus.vDeployments[d].nFalloffCoeff = nFalloffCoeff;
    }
}

void CChainParams::UpdateDIP3Parameters(int nActivationHeight, int nEnforcementHeight)
{
    consensus.DIP0003Height = nActivationHeight;
    consensus.DIP0003EnforcementHeight = nEnforcementHeight;
}

void CChainParams::UpdateBudgetParameters(int nMasternodePaymentsStartBlock, int nBudgetPaymentsStartBlock, int nSuperblockStartBlock)
{
    consensus.nMasternodePaymentsStartBlock = nMasternodePaymentsStartBlock;
    consensus.nBudgetPaymentsStartBlock = nBudgetPaymentsStartBlock;
    consensus.nSuperblockStartBlock = nSuperblockStartBlock;
}

void CChainParams::UpdateSubsidyAndDiffParams(int nMinimumDifficultyBlocks, int nHighSubsidyBlocks, int nHighSubsidyFactor)
{
    consensus.nMinimumDifficultyBlocks = nMinimumDifficultyBlocks;
    consensus.nHighSubsidyBlocks = nHighSubsidyBlocks;
    consensus.nHighSubsidyFactor = nHighSubsidyFactor;
}

void CChainParams::UpdateLLMQChainLocks(Consensus::LLMQType llmqType) {
    consensus.llmqTypeChainLocks = llmqType;
}

void CChainParams::UpdateLLMQTestParams(int size, int threshold) {
    auto& params = consensus.llmqs.at(Consensus::LLMQ_TEST);
    params.size = size;
    params.minSize = threshold;
    params.threshold = threshold;
    params.dkgBadVotesThreshold = threshold;
}

void CChainParams::UpdateLLMQDevnetParams(int size, int threshold)
{
    auto& params = consensus.llmqs.at(Consensus::LLMQ_DEVNET);
    params.size = size;
    params.minSize = threshold;
    params.threshold = threshold;
    params.dkgBadVotesThreshold = threshold;
}

static CBlock FindDevNetGenesisBlock(const CBlock &prevBlock, const CAmount& reward)
{
    std::string devNetName = gArgs.GetDevNetName();
    assert(!devNetName.empty());

    CBlock block = CreateDevNetGenesisBlock(prevBlock.GetHash(), devNetName.c_str(), prevBlock.nTime + 1, 0, prevBlock.nBits, reward);

    arith_uint256 bnTarget;
    bnTarget.SetCompact(block.nBits);

    for (uint32_t nNonce = 0; nNonce < UINT32_MAX; nNonce++) {
        block.nNonce = nNonce;

        uint256 hash = block.GetHash();
        if (UintToArith256(hash) <= bnTarget)
            return block;
    }

    // This is very unlikely to happen as we start the devnet with a very low difficulty. In many cases even the first
    // iteration of the above loop will give a result already
    error("FindDevNetGenesisBlock: could not find devnet genesis block for %s", devNetName);
    assert(false);
}

// this one is for testing only
static Consensus::LLMQParams llmq_test = {
        .type = Consensus::LLMQ_TEST,
        .name = "llmq_test",
        .size = 3,
        .minSize = 2,
        .threshold = 2,

        .dkgInterval = 24, // one DKG per hour
        .dkgPhaseBlocks = 2,
        .dkgMiningWindowStart = 10, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 18,
        .dkgBadVotesThreshold = 2,

        .signingActiveQuorumCount = 2, // just a few ones to allow easier testing

        .keepOldConnections = 3,
        .recoveryMembers = 3,
};

// this one is for devnets only
static Consensus::LLMQParams llmq_devnet = {
        .type = Consensus::LLMQ_DEVNET,
        .name = "llmq_devnet",
        .size = 10,
        .minSize = 7,
        .threshold = 6,

        .dkgInterval = 24, // one DKG per hour
        .dkgPhaseBlocks = 2,
        .dkgMiningWindowStart = 10, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 18,
        .dkgBadVotesThreshold = 7,

        .signingActiveQuorumCount = 3, // just a few ones to allow easier testing

        .keepOldConnections = 4,
        .recoveryMembers = 6,
};

static Consensus::LLMQParams llmq50_60 = {
        .type = Consensus::LLMQ_50_60,
        .name = "llmq_50_60",
        .size = 50,
        .minSize = 40,
        .threshold = 30,

        .dkgInterval = 24, // one DKG per hour
        .dkgPhaseBlocks = 2,
        .dkgMiningWindowStart = 10, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 18,
        .dkgBadVotesThreshold = 40,

        .signingActiveQuorumCount = 24, // a full day worth of LLMQs

        .keepOldConnections = 25,
        .recoveryMembers = 25,
};

static Consensus::LLMQParams llmq400_60 = {
        .type = Consensus::LLMQ_400_60,
        .name = "llmq_400_60",
        .size = 400,
        .minSize = 300,
        .threshold = 240,

        .dkgInterval = 24 * 12, // one DKG every 12 hours
        .dkgPhaseBlocks = 4,
        .dkgMiningWindowStart = 20, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 28,
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // two days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,
};

// Used for deployment and min-proto-version signalling, so it needs a higher threshold
static Consensus::LLMQParams llmq400_85 = {
        .type = Consensus::LLMQ_400_85,
        .name = "llmq_400_85",
        .size = 400,
        .minSize = 350,
        .threshold = 340,

        .dkgInterval = 24 * 24, // one DKG every 24 hours
        .dkgPhaseBlocks = 4,
        .dkgMiningWindowStart = 20, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 48, // give it a larger mining window to make sure it is mined
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // four days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,
};


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210240; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 2; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 158000; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - actual historical value
        consensus.nInstantSendConfirmationsRequired = 6;
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 328008; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nSuperblockStartBlock = 0; // The block at which 12.1 goes live (end of final 12.0 budget cycle)
        consensus.nSuperblockStartHash = uint256S("00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6");
        consensus.nSuperblockCycle = 16616; // ~(60*24*30)/2.6, actual number of blocks per month is 200700 / 12 = 16725
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6");
        consensus.BIP65Height = 0;     // 00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6
        consensus.BIP66Height = 0;     // 00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6
        consensus.DIP0001Height = 10;  // 00000056cfbdf18e4890e10a6850943f11e44da5dcaf9b82d9320b99f2fffde3
        consensus.DIP0003Height = 20;  // 000003e02c207c290aba728ffe961b7797e32efc61e985aca036b48ac50480cb
        consensus.DIP0003EnforcementHeight = 30;
        consensus.DIP0003EnforcementHash = uint256S("0000047b2bb97f484c45bf6e18e64fcc5fae8b4043893e01f48a43500ba10617");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 60; // Xazab: 1 minutes
        consensus.nPowTargetSpacing = 60; // Xazab:  1 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 5;
        consensus.nPowDGWHeight = 6;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1390095618; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1390095618; // Oct 15th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1539561600; // Oct 15th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThresholdStart = 3226; // 80% of 4032

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 1390095618; // Apr 23th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 1556013600; // Apr 23th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nThresholdStart = 3226; // 80% of 4032

        // Deployment of DIP0003
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nStartTime = 1390095618; // Jan 1st, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nTimeout = 1577836800; // Jan 1st, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nThresholdStart = 3226; // 80% of 4032

        // Deployment of DIP0008
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nStartTime = 1557878400; // May 15th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nTimeout = 1589500800; // May 15th, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nThresholdStart = 3226; // 80% of 4032

        // Deployment of Block Reward Reallocation
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nStartTime = 1390095618; // Oct 1st, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nTimeout = 1633046400; // Oct 1st, 2021
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nWindowSize = 4032;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdStart = 3226; // 80% of 4032
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdMin = 2420; // 60% of 4032
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nFalloffCoeff = 5; // this corresponds to 10 periods

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S(""); // 1344000

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S(""); // 1344000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x55;
        pchMessageStart[1] = 0x46;
        pchMessageStart[2] = 0x58;
        pchMessageStart[3] = 0x50;
        nDefaultPort = 30303;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1390095618, 28917698, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6"));
        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("dnsseed.xazab.xyz");
        vSeeds.emplace_back("dnsseed1.xazab.xyz");

        // Xazab addresses start with 'X'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,76);
        // Xazab script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
        // Xazab private keys start with '7' or 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        // Xazab BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        // Xazab BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // Xazab BIP44 coin type is '988'
        nExtCoinType = 988;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;
        consensus.llmqTypeChainLocks = Consensus::LLMQ_400_60;
        consensus.llmqTypeInstantSend = Consensus::LLMQ_50_60;

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fRequireRoutableExternalIP = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = false;
        nLLMQConnectionRetryTimeout = 60;

        nPoolMinParticipants = 3;
        nPoolNewMinParticipants = 3;
        nPoolMaxParticipants = 5;
        nPoolNewMaxParticipants = 20;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour

        vSporkAddresses = {"XnGqZfBZ9TGmDykH12Ryb2u763Ydto4RiP"};
        nMinSporkKeys = 1;
        fBIP9CheckMasternodesUpgraded = true;

        checkpointData = {
            {
                {0,   uint256S("0x00000ffd590b1485b3caadc19b22e6379c733355108f107a430458cdf3407ab6")},
                {10,  uint256S("0x00000056cfbdf18e4890e10a6850943f11e44da5dcaf9b82d9320b99f2fffde3")},
                {100, uint256S("0x00000832d1e15c23c4064b9d1ca459f16c7aeb27eebd7b46150d026848dc3738")},
                {150, uint256S("0x000004163822db7d7686d98d9c896c5b6c5dfdcb52e6316e0d4f83fd0d2a124e")},
                {200, uint256S("0x0000074cd14f9248f611480411d29fc0c659ab4be4086bc99557a5792ee92147")},
                {300, uint256S("0x000002613222721f35d43170497363813bf19835b2050ac4444031d4a597f324")},
            }
        };

        chainTxData = ChainTxData{
           1602749454,          // * UNIX timestamp of last known number of transactions (Block 1344000)
           476,                 // * total number of transactions between genesis and that timestamp
                                //   (the tx=... number in the SetBestChain debug.log lines)
           0.01680314793151122  // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockStartHash = uint256(); // do not check this on testnet
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("");
        consensus.BIP65Height = 88;
        consensus.BIP66Height = 98;
        consensus.DIP0001Height = 100;
        consensus.DIP0003Height = 150;
        consensus.DIP0003EnforcementHeight = 160;
        consensus.DIP0003EnforcementHash = uint256S("");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 60; // Xazab: 1 minutes
        consensus.nPowTargetSpacing =  60; // Xazab: 1 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 42; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 42; // TODO: make sure to drop all spork6 related code on next testnet reset
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1544655600; // Dec 13th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1576191600; // Dec 13th, 2019

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1544655600; // Dec 13th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1576191600; // Dec 13th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThresholdStart = 50; // 50% of 100

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 1544655600; // Dec 13th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 1576191600; // Dec 13th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nThresholdStart = 50; // 50% of 100

        // Deployment of DIP0003
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nStartTime = 1544655600; // Dec 13th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nTimeout = 1576191600; // Dec 13th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nThresholdStart = 50; // 50% of 100

        // Deployment of DIP0008
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nStartTime = 1553126400; // Mar 21st, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nTimeout = 1584748800; // Mar 21st, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nThresholdStart = 50; // 50% of 100

        // Deployment of Block Reward Reallocation
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nStartTime = 1598918400; // Sep 1st, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nTimeout = 1630454400; // Sep 1st, 2021
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdStart = 80; // 80% of 100
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdMin = 60; // 60% of 100
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nFalloffCoeff = 5; // this corresponds to 10 periods

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("");

        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x46;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xff;
        nDefaultPort = 29999;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1390666206UL, 3861367235UL, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000bafbc94add76cb75e2ec92894837288a481e5c005f6563d91623bf8bc2c"));
        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        vFixedSeeds.clear();
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.xazab.xyz"); // Just a static list of stable node(s), only supports x9

        // Testnet Xazab addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Testnet Xazab script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet Xazab BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        // Testnet Xazab BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Testnet Xazab BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;
        consensus.llmqTypeChainLocks = Consensus::LLMQ_50_60;
        consensus.llmqTypeInstantSend = Consensus::LLMQ_50_60;

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fRequireRoutableExternalIP = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = true;
        nLLMQConnectionRetryTimeout = 60;

        nPoolMinParticipants = 3;
        nPoolNewMinParticipants = 2;
        nPoolMaxParticipants = 5;
        nPoolNewMaxParticipants = 20;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        vSporkAddresses = {"yjPtiKh2uwk3bDutTEA2q9mCtXyiZRWn55"};
        nMinSporkKeys = 1;
        fBIP9CheckMasternodesUpgraded = true;

        checkpointData = {
            {
                {0, uint256S("0x00000bafbc94add76cb75e2ec92894837288a481e5c005f6563d91623bf8bc2c")},
            }
        };

        chainTxData = ChainTxData{
            1390666206, // * UNIX timestamp of last known number of transactions (Block 387900)
            0,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0        // * estimated number of transactions per second after that timestamp
        };

    }
};

/**
 * Devnet
 */
class CDevNetParams : public CChainParams {
public:
    CDevNetParams() {
        strNetworkID = "devnet";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockStartHash = uint256(); // do not check this on devnet
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on devnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 1; // BIP34 activated immediately on devnet
        consensus.BIP65Height = 1; // BIP65 activated immediately on devnet
        consensus.BIP66Height = 1; // BIP66 activated immediately on devnet
        consensus.DIP0001Height = 2; // DIP0001 activated immediately on devnet
        consensus.DIP0003Height = 2; // DIP0003 activated immediately on devnet
        consensus.DIP0003EnforcementHeight = 2; // DIP0003 activated immediately on devnet
        consensus.DIP0003EnforcementHash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 60; // Xazab: 1 minutes
        consensus.nPowTargetSpacing = 60; // Xazab: 1 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 4001;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThresholdStart = 50; // 50% of 100

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 1517792400; // Feb 5th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 1549328400; // Feb 5th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nThresholdStart = 50; // 50% of 100

        // Deployment of DIP0003
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nStartTime = 1535752800; // Sep 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nTimeout = 1567288800; // Sep 1st, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nThresholdStart = 50; // 50% of 100

        // Deployment of DIP0008
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nStartTime = 1553126400; // Mar 21st, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nTimeout = 1900281600; // Mar 21st, 2030
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nThresholdStart = 50; // 50% of 100

        // Deployment of Block Reward Reallocation
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nStartTime = 1598918400; // Sep 1st, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nTimeout = 1900281600; // Mar 21st, 2030
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdStart = 80; // 80% of 100
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdMin = 60; // 60% of 100
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nFalloffCoeff = 5; // this corresponds to 10 periods

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000000000000000000000000000000000000000000000000000000000");

        pchMessageStart[0] = 0x49;
        pchMessageStart[1] = 0x65;
        pchMessageStart[2] = 0x52;
        pchMessageStart[3] = 0xce;
        nDefaultPort = 29799;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1417713337, 1096447, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"));
        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        devnetGenesis = FindDevNetGenesisBlock(genesis, 50 * COIN);
        consensus.hashDevnetGenesisBlock = devnetGenesis.GetHash();

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("xazabevo.org",  "devnet-seed.xazabevo.org"));

        // Testnet Xazab addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Testnet Xazab script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet Xazab BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        // Testnet Xazab BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Testnet Xazab BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_DEVNET] = llmq_devnet;
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;
        consensus.llmqTypeChainLocks = Consensus::LLMQ_50_60;
        consensus.llmqTypeInstantSend = Consensus::LLMQ_50_60;

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fRequireRoutableExternalIP = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = true;
        nLLMQConnectionRetryTimeout = 60;

        nPoolMinParticipants = 3;
        nPoolNewMinParticipants = 2;
        nPoolMaxParticipants = 5;
        nPoolNewMaxParticipants = 20;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        vSporkAddresses = {"yjPtiKh2uwk3bDutTEA2q9mCtXyiZRWn55"};
        nMinSporkKeys = 1;
        // devnets are started with no blocks and no MN, so we can't check for upgraded MN (as there are none)
        fBIP9CheckMasternodesUpgraded = false;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e")},
                { 1, devnetGenesis.GetHash() },
            }
        };

        chainTxData = ChainTxData{
            devnetGenesis.GetBlockTime(), // * UNIX timestamp of devnet genesis block
            2,                            // * we only have 2 coinbase transactions when a devnet is started up
            0.01                          // * estimated number of transactions per second
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockStartHash = uint256(); // do not check this on regtest
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.DIP0001Height = 2000;
        consensus.DIP0003Height = 432;
        consensus.DIP0003EnforcementHeight = 500;
        consensus.DIP0003EnforcementHash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Xazab: 1 day
        consensus.nPowTargetSpacing = 2.5 * 60; // Xazab: 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 34140; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].bit = 3;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0003].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0008].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nWindowSize = 500;
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdStart = 400; // 80%
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nThresholdMin = 300; // 60%
        consensus.vDeployments[Consensus::DEPLOYMENT_REALLOC].nFalloffCoeff = 5;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x65;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0x62;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 29899;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1417713337, 1096447, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"));
        assert(genesis.hashMerkleRoot == uint256S("0xe0028eb9648db56b1ac77cf090b99048a8007e2bb64b68f092c03c7f56a662c7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fRequireRoutableExternalIP = false;
        fMineBlocksOnDemand = true;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = true;
        nLLMQConnectionRetryTimeout = 1; // must be lower then the LLMQ signing session timeout so that tests have control over failing behavior

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        nPoolMinParticipants = 2;
        nPoolNewMinParticipants = 2;
        nPoolMaxParticipants = 5;
        nPoolNewMaxParticipants = 20;

        // privKey: cP4EKFyJsHT39LDqgdcB43Y3YXjNyjb5Fuas1GQSeAtjnZWmZEQK
        vSporkAddresses = {"yj949n1UH6fDhw6HtVE5VMj2iSTaSWBMcW"};
        nMinSporkKeys = 1;
        // regtest usually has no masternodes in most tests, so don't check for upgraged MNs
        fBIP9CheckMasternodesUpgraded = false;

        checkpointData = {
            {
                {0, uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        // Regtest Xazab addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Regtest Xazab script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest Xazab BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        // Regtest Xazab BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Regtest Xazab BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_TEST] = llmq_test;
        consensus.llmqTypeChainLocks = Consensus::LLMQ_TEST;
        consensus.llmqTypeInstantSend = Consensus::LLMQ_TEST;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::DEVNET) {
        return std::unique_ptr<CChainParams>(new CDevNetParams());
    } else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout, int64_t nWindowSize, int64_t nThresholdStart, int64_t nThresholdMin, int64_t nFalloffCoeff)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout, nWindowSize, nThresholdStart, nThresholdMin, nFalloffCoeff);
}

void UpdateDIP3Parameters(int nActivationHeight, int nEnforcementHeight)
{
    globalChainParams->UpdateDIP3Parameters(nActivationHeight, nEnforcementHeight);
}

void UpdateBudgetParameters(int nMasternodePaymentsStartBlock, int nBudgetPaymentsStartBlock, int nSuperblockStartBlock)
{
    globalChainParams->UpdateBudgetParameters(nMasternodePaymentsStartBlock, nBudgetPaymentsStartBlock, nSuperblockStartBlock);
}

void UpdateDevnetSubsidyAndDiffParams(int nMinimumDifficultyBlocks, int nHighSubsidyBlocks, int nHighSubsidyFactor)
{
    globalChainParams->UpdateSubsidyAndDiffParams(nMinimumDifficultyBlocks, nHighSubsidyBlocks, nHighSubsidyFactor);
}

void UpdateDevnetLLMQChainLocks(Consensus::LLMQType llmqType)
{
    globalChainParams->UpdateLLMQChainLocks(llmqType);
}

void UpdateLLMQTestParams(int size, int threshold)
{
    globalChainParams->UpdateLLMQTestParams(size, threshold);
}

void UpdateLLMQDevnetParams(int size, int threshold)
{
    globalChainParams->UpdateLLMQDevnetParams(size, threshold);
}
