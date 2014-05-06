#include "nlsr.hpp"
#include "sync-logic-handler.hpp"
#include "utility/name-helper.hpp"
#include "lsa.hpp"


namespace nlsr {

using namespace ndn;
using namespace std;

void
SyncLogicHandler::createSyncSocket(Nlsr& pnlsr)
{
  std::cout << "Creating Sync socket ......" << std::endl;
  std::cout << "Sync prefix: " << m_syncPrefix << std::endl;
  m_syncSocket = make_shared<Sync::SyncSocket>(m_syncPrefix, m_validator,
                                               m_syncFace,
                                               bind(&SyncLogicHandler::nsyncUpdateCallBack, this,
                                                    _1, _2, boost::ref(pnlsr)),
                                               bind(&SyncLogicHandler::nsyncRemoveCallBack, this,
                                                    _1, boost::ref(pnlsr)));
}

void
SyncLogicHandler::nsyncUpdateCallBack(const vector<Sync::MissingDataInfo>& v,
                                      Sync::SyncSocket* socket, Nlsr& pnlsr)
{
  std::cout << "nsyncUpdateCallBack called ...." << std::endl;
  int32_t n = v.size();
  for (int32_t i = 0; i < n; i++)
  {
    std::cout << "Data Name: " << v[i].prefix << " Seq: " << v[i].high.getSeq() <<
              endl;
    processUpdateFromSync(v[i].prefix, v[i].high.getSeq(), pnlsr);
  }
}

void
SyncLogicHandler::nsyncRemoveCallBack(const string& prefix, Nlsr& pnlsr)
{
  std::cout << "nsyncRemoveCallBack called ...." << std::endl;
}

void
SyncLogicHandler::removeRouterFromSyncing(const ndn::Name& routerPrefix)
{
}

void
SyncLogicHandler::processUpdateFromSync(const ndn::Name& updateName,
                                        uint64_t seqNo,  Nlsr& pnlsr)
{
  //const ndn::Name name(updateName);
  string chkString("LSA");
  int32_t lasPosition = util::getNameComponentPosition(updateName, chkString);
  if (lasPosition >= 0)
  {
    ndn::Name routerName = updateName.getSubName(lasPosition + 1);
    processRoutingUpdateFromSync(routerName, seqNo, pnlsr);
    return;
  }
}

void
SyncLogicHandler::processRoutingUpdateFromSync(const ndn::Name& routerName,
                                               uint64_t seqNo,  Nlsr& pnlsr)
{
  ndn::Name rName = routerName;
  if (routerName != pnlsr.getConfParameter().getRouterPrefix())
  {
    SequencingManager sm(seqNo);
    std::cout << sm;
    std::cout << "Router Name: " << routerName << endl;
    try
    {
      if (pnlsr.getLsdb().isNameLsaNew(rName.append("name"), sm.getNameLsaSeq()))
      {
        std::cout << "Updated Name LSA. Need to fetch it" << std::endl;
        ndn::Name interestName(pnlsr.getConfParameter().getChronosyncLsaPrefix());
        interestName.append(routerName);
        interestName.append("name");
        interestName.appendNumber(sm.getNameLsaSeq());
        pnlsr.getLsdb().expressInterest(interestName,
                                        pnlsr.getConfParameter().getInterestResendTime());
      }
      if (pnlsr.getLsdb().isAdjLsaNew(rName.append("adjacency"),
                                      sm.getAdjLsaSeq()))
      {
        std::cout << "Updated Adj LSA. Need to fetch it" << std::endl;
        ndn::Name interestName(pnlsr.getConfParameter().getChronosyncLsaPrefix());
        interestName.append(routerName);
        interestName.append("adjacency");
        interestName.appendNumber(sm.getAdjLsaSeq());
        pnlsr.getLsdb().expressInterest(interestName,
                                        pnlsr.getConfParameter().getInterestResendTime());
      }
      if (pnlsr.getLsdb().isCoordinateLsaNew(rName.append("coordinate"),
                                             sm.getCorLsaSeq()))
      {
        std::cout << "Updated Cor LSA. Need to fetch it" << std::endl;
        ndn::Name interestName(pnlsr.getConfParameter().getChronosyncLsaPrefix());
        interestName.append(routerName);
        interestName.append("coordinate");
        interestName.appendNumber(sm.getCorLsaSeq());
        pnlsr.getLsdb().expressInterest(interestName,
                                        pnlsr.getConfParameter().getInterestResendTime());
      }
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return;
    }
  }
}

void
SyncLogicHandler::publishRoutingUpdate(SequencingManager& sm,
                                       const ndn::Name& updatePrefix)
{
  sm.writeSeqNoToFile();
  publishSyncUpdate(updatePrefix, sm.getCombinedSeqNo());
}

void
SyncLogicHandler::publishSyncUpdate(const ndn::Name& updatePrefix,
                                    uint64_t seqNo)
{
  std::cout << "Publishing Sync Update ......" << std::endl;
  std::cout << "Update in prefix: " << updatePrefix << std::endl;
  std::cout << "Seq No: " << seqNo << std::endl;
  ndn::Name updateName(updatePrefix);
  string data("NoData");
  m_syncSocket->publishData(updateName.toUri(), 0, data.c_str(), data.size(),
                            1000,
                            seqNo);
}

}//namespace nlsr
