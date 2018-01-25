//
// MIT License
//
// Copyright (c) 2017-2018 Thibault Martinez and Simon Ninon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//

#include <gtest/gtest.h>

#include <iota/api/extended.hpp>
#include <iota/errors/illegal_state.hpp>
#include <test/utils/configuration.hpp>
#include <test/utils/constants.hpp>
#include <test/utils/expect_exception.hpp>

TEST(Extended, PrepareTransfers) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 4UL);

  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 3);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 3);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 3);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 3);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersWithChecksum) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 4UL);
  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 3);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 3);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 3);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 3);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());

  //! should have remove checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersNoRemainderAddress) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers, "", inputs, true);

  ASSERT_EQ(res.size(), 4UL);
  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 3);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 3);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 3);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 3);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_7_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersNoTransfer) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfers = std::vector<IOTA::Models::Transfer>{};

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 0UL);
}

TEST(Extended, PrepareTransfersInvalidSeed) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers("hello", 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Seed");
}

TEST(Extended, PrepareTransfersInvalidTransferAddress) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ "HELLO", 100, "TESTMSG", "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Transfer");
}

TEST(Extended, PrepareTransfersInvalidTransferTag) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "invalid__" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Transfer");
}

TEST(Extended, PrepareTransfersInvalidTransferMsg) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100,
                                          "invalid__", "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Transfer");
}

TEST(Extended, PrepareTransfersInvalidRemainder) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers, "hello", inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Remainder");
}

TEST(Extended, PrepareTransfersNotEnoughFund) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                          ACCOUNT_1_ADDRESS_1_FUND + 1, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Not enough balance");
}

TEST(Extended, PrepareTransfersNotEnoughFundWithoutCheckAndFakeBalance) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                          ACCOUNT_1_ADDRESS_1_FUND + 1, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND + 1, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, false);

  ASSERT_EQ(res.size(), 3UL);
  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 2);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), ACCOUNT_1_ADDRESS_1_FUND + 1);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 2);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -(ACCOUNT_1_ADDRESS_1_FUND + 1));
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 2);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersNotEnoughFundWithFakeBalanceAndCheck) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                          ACCOUNT_1_ADDRESS_1_FUND + 1, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND + 1, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Not enough balance");
}

TEST(Extended, PrepareTransfersNotKnowingBalance) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 0, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 4UL);
  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 3);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 3);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 3);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 3);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersNoInput) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto inputs = std::vector<IOTA::Models::Input>{};

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 4UL);

  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 3);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 3);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 3);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 3);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersInputSecurity) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 3 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 3, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 5UL);

  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 4);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 100);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  auto trx2 = IOTA::Models::Transaction{ res[1] };
  EXPECT_EQ(trx2.getCurrentIndex(), 1);
  EXPECT_EQ(trx2.getLastIndex(), 4);
  EXPECT_EQ(trx2.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx2.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx2.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx2.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx2.getValue(), -ACCOUNT_1_ADDRESS_1_FUND);
  EXPECT_EQ(trx2.getPersistence(), false);
  EXPECT_TRUE(trx2.getTimestamp() > 0);
  EXPECT_FALSE(trx2.getHash().empty());
  EXPECT_FALSE(trx2.getBundle().empty());
  EXPECT_FALSE(trx2.getSignatureFragments().empty());

  auto trx3 = IOTA::Models::Transaction{ res[2] };
  EXPECT_EQ(trx3.getCurrentIndex(), 2);
  EXPECT_EQ(trx3.getLastIndex(), 4);
  EXPECT_EQ(trx3.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx3.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx3.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx3.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx3.getValue(), 0);
  EXPECT_EQ(trx3.getPersistence(), false);
  EXPECT_TRUE(trx3.getTimestamp() > 0);
  EXPECT_FALSE(trx3.getHash().empty());
  EXPECT_FALSE(trx3.getBundle().empty());
  EXPECT_FALSE(trx3.getSignatureFragments().empty());

  auto trx4 = IOTA::Models::Transaction{ res[3] };
  EXPECT_EQ(trx4.getCurrentIndex(), 3);
  EXPECT_EQ(trx4.getLastIndex(), 4);
  EXPECT_EQ(trx4.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx4.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx4.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx4.getAddress(), ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx4.getValue(), 0);
  EXPECT_EQ(trx4.getPersistence(), false);
  EXPECT_TRUE(trx4.getTimestamp() > 0);
  EXPECT_FALSE(trx4.getHash().empty());
  EXPECT_FALSE(trx4.getBundle().empty());
  EXPECT_FALSE(trx4.getSignatureFragments().empty());

  auto trx5 = IOTA::Models::Transaction{ res[4] };
  EXPECT_EQ(trx5.getCurrentIndex(), 4);
  EXPECT_EQ(trx5.getLastIndex(), 4);
  EXPECT_EQ(trx5.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx5.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx5.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx5.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx5.getAddress(), ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx5.getValue(), ACCOUNT_1_ADDRESS_1_FUND - 100);
  EXPECT_EQ(trx5.getPersistence(), false);
  EXPECT_TRUE(trx5.getTimestamp() > 0);
  EXPECT_FALSE(trx5.getHash().empty());
  EXPECT_FALSE(trx5.getBundle().empty());
  EXPECT_FALSE(trx5.getSignatureFragments().empty());

  //! should have same bundle
  EXPECT_EQ(trx1.getBundle(), trx2.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx3.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx4.getBundle());
  EXPECT_EQ(trx1.getBundle(), trx5.getBundle());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersInvalidSecurityLevel) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 100, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  EXPECT_EXCEPTION(api.prepareTransfers(ACCOUNT_1_SEED, 42, transfers,
                                        ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true),
                   IOTA::Errors::IllegalState, "Invalid Security Level");
}

TEST(Extended, PrepareTransfersZeroTransfer) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 0, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto input  = IOTA::Models::Input{ ACCOUNT_1_ADDRESS_1_HASH_WITHOUT_CHECKSUM,
                                    ACCOUNT_1_ADDRESS_1_FUND, 0, 2 };
  auto inputs = std::vector<IOTA::Models::Input>{ input };

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 1UL);

  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 0);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 0);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}

TEST(Extended, PrepareTransfersZeroTransferNoInput) {
  auto api = IOTA::API::Extended{ get_proxy_host(), get_proxy_port() };

  auto transfer  = IOTA::Models::Transfer{ ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM, 0, "TESTMSG",
                                          "TESTTAG99999999999999999999" };
  auto transfers = std::vector<IOTA::Models::Transfer>{ transfer };

  auto inputs = std::vector<IOTA::Models::Input>{};

  auto res = api.prepareTransfers(ACCOUNT_1_SEED, 2, transfers,
                                  ACCOUNT_1_ADDRESS_2_HASH_WITHOUT_CHECKSUM, inputs, true);

  ASSERT_EQ(res.size(), 1UL);

  auto trx1 = IOTA::Models::Transaction{ res[0] };
  EXPECT_EQ(trx1.getCurrentIndex(), 0);
  EXPECT_EQ(trx1.getLastIndex(), 0);
  EXPECT_EQ(trx1.getTag(), "TESTTAG99999999999999999999");
  EXPECT_EQ(trx1.getNonce(), IOTA::EmptyNonce);
  EXPECT_EQ(trx1.getTrunkTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getBranchTransaction(), IOTA::EmptyHash);
  EXPECT_EQ(trx1.getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
  EXPECT_EQ(trx1.getValue(), 0);
  EXPECT_EQ(trx1.getPersistence(), false);
  EXPECT_TRUE(trx1.getTimestamp() > 0);
  EXPECT_FALSE(trx1.getHash().empty());
  EXPECT_FALSE(trx1.getBundle().empty());
  EXPECT_FALSE(trx1.getSignatureFragments().empty());

  //! should keep without the checksum
  EXPECT_EQ(transfers[0].getAddress(), ACCOUNT_2_ADDRESS_1_HASH_WITHOUT_CHECKSUM);
}