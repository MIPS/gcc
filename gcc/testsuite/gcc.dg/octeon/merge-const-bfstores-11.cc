/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not ",47\[^0-9\]" } } */

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
namespace LteApp
{
  namespace R2
  {
    class VariableContainer
    {
    public:VariableContainer ();
      ~VariableContainer ();
    };
  }
  namespace R2RRC
  {
    typedef uint16_t SrbIdentity;
    typedef uint16_t EpsBearerIdentity;
    typedef uint16_t DrbIdentity;
    typedef uint16_t DtchLogicalChannelId;
    const uint16_t MIN_DTCH_LOGICAL_CHN_ID = 3;
    const uint16_t MIN_DRB = 1;
    const uint16_t MAX_DRB = 11;
    const uint16_t MIN_EPS_BEARER_ID = 0;
    const uint16_t MIN_DRB_ID = 1;
    const uint16_t MAX_MEAS_OBJECT_ID = 32;
    const uint16_t INIT_VALUE_16BITS = 0xffff;
      template < class T, uint16_t low, uint16_t up,
      bool resetOk > class CommonList;
      template < class T, uint16_t low, uint16_t up > class CommonList < T,
      low, up, true >
    {
    public:typedef T innerType_;
        CommonList ();
      uint16_t numElem_;
      T list_[up];
    };
      template < class T, uint16_t low, uint16_t up > inline CommonList < T,
      low, up, true >::CommonList ():numElem_ (0)
    {
    }
    template < class T, uint16_t low, uint16_t up > class CommonList < T, low,
      up, false >
    {
      T list_[up];
    };
    template < uint16_t No > class EmptyType;
    template < class T1 = EmptyType < 1 >, class T2 =
      EmptyType < 2 >, class T3 = EmptyType < 3 >, class T4 =
      EmptyType < 4 >, class T5 = EmptyType < 5 >, class T6 =
      EmptyType < 6 >, class T7 = EmptyType < 7 >, class T8 =
      EmptyType < 8 >, class T9 = EmptyType < 9 >, class T10 =
      EmptyType < 10 > >class UnionContainer
    {
    public:template < class Obj, class T,
	bool isEmptyType > friend class SelectType;
      typedef T2 innerType2;
      enum TypeChoice
      {
	UNSPEC_CHOSEN = 0, TYPE1_CHOSEN = 1, TYPE2_CHOSEN = 2, TYPE3_CHOSEN =
	  9, TYPE10_CHOSEN = 10
      };
      ~UnionContainer ();
      union
      {
	uint8_t buf2_[sizeof (innerType2)];
      }
      u;
      TypeChoice choice_;
    };
    template < class T1, class T2, class T3, class T4, class T5, class T6,
      class T7, class T8, class T9, class T10 > inline UnionContainer < T1,
      T2, T3, T4, T5, T6, T7, T8, T9, T10 >::~UnionContainer ()
    {
      switch (choice_)
	{
	case TYPE1_CHOSEN:
	  {
	    reinterpret_cast < T2 * >(u.buf2_)->~T2 ();
	  }
	}
      choice_ = UNSPEC_CHOSEN;
    }
    typedef LteApp::R2::VariableContainer NasDedicatedInformation;
    enum NumOfRaPreambles
    {
      RA_PREAMBLES_N4 = 0, RA_PREAMBLES_N8 = 1, RA_PREAMBLES_N12 = 2, RA_PREAMBLES_N16 = 3, RA_PREAMBLES_N20 = 4, RA_PREAMBLES_N24 = 5, RA_PREAMBLES_N28 = 6, RA_PREAMBLES_N32 = 7, RA_PREAMBLES_N36 = 8, RA_PREAMBLES_N40 = 9, RA_PREAMBLES_N44 = 10, RA_PREAMBLES_N48 = 11, RA_PREAMBLES_N52 = 12, RA_PREAMBLES_N56 = 13, RA_PREAMBLES_N60 = 14, RA_PREAMBLES_N64 = 15,	//            NUM_OF_RA_PREAMBLES,
      NUM_OF_RA_PREAMBLES = 32, MIN_RA_PREAMBLES =
	RA_PREAMBLES_N4, MAX_RA_PREAMBLES = NUM_OF_RA_PREAMBLES - 1
    };
    class PreambleInfo
    {
    public:PreambleInfo ();
      NumOfRaPreambles numOfRaPreambles_;
      bool isSizeOfRaPreamblesGroupAPresent_;
      NumOfRaPreambles sizeOfRaPreamblesGroupA_;
    };
    inline PreambleInfo::PreambleInfo (): /* XXXX  *  * Here, the constructor for PreambleInfo initializes  numOfRaPreambles_  * to NUM_OF_RA_PREAMBLES, which is 32 (0x20).  */ numOfRaPreambles_ (NUM_OF_RA_PREAMBLES),	//        numOfRaPreambles_(/*NUM_OF_RA_PREAMBLES*/(LteApp::R2RRC::NumOfRaPreambles)0x20),
      sizeOfRaPreamblesGroupA_ (NUM_OF_RA_PREAMBLES)
    {
    }
    enum PowerRampingStep
    {
      PW_RAMPING_STEP_DB0 = 0, PW_RAMPING_STEP_DB2 = 1, PW_RAMPING_STEP_DB4 =
	2, PW_RAMPING_STEP_DB6 = 3, NUM_PW_RAMPING_STEP
    };
    enum PreambleInitialReceivedTargetPower
    {
      REAMBLE_INIT_RECV_TGT_PW_DBM_120 = 0, REAMBLE_INIT_RECV_TGT_PW_DBM_118 =
	7, REAMBLE_INIT_RECV_TGT_PW_DBM_104 =
	15, NUM_PREAMBLE_INIT_RECV_TGT_PW
    };
    class PowerRampingParameters
    {
    public:PowerRampingParameters ();
      static const PreambleInitialReceivedTargetPower
	DEFAULT_PREAMBLE_INIT_RECV_TGT_POWER =
	REAMBLE_INIT_RECV_TGT_PW_DBM_104;
      PowerRampingStep powerRampingStep_;
      PreambleInitialReceivedTargetPower preambleInitialReceivedTargetPower_;
    };
    inline PowerRampingParameters::
      PowerRampingParameters ():powerRampingStep_ (PW_RAMPING_STEP_DB0),
      preambleInitialReceivedTargetPower_
      (DEFAULT_PREAMBLE_INIT_RECV_TGT_POWER)
    {
    }
    enum PreambleTransMax
    {
      PREAMBLE_TRANS_MAX_N1 = 0, PREAMBLE_TRANS_MAX_N2 =
	15, NUM_PREAMBLE_TRNAS_MAX
    };
    enum RaResponseWindowSize
    {
      RA_RESP_WINDOWSIZE_SF2 = 0, RA_RESP_WINDOWSIZE_SF3 =
	7, NUM_RA_RESP_WIN_SIZE
    };
    enum MacContentionResolutionTimer
    {
      MAC_CONT_RESOL_TIMER_SF8 = 0, MAC_CONT_RESOL_TIMER_SF16 =
	7, NUM_MAC_CONT_RESOL_TIMER
    };
    class RaSupervisionInfo
    {
    public:RaSupervisionInfo ();
      PreambleTransMax preambleTransMax_;
      RaResponseWindowSize raResponseWindowSize_;
      MacContentionResolutionTimer macContentionResolutionTimer_;
    };
    inline RaSupervisionInfo::
      RaSupervisionInfo ():preambleTransMax_ (PREAMBLE_TRANS_MAX_N1),
      macContentionResolutionTimer_ (MAC_CONT_RESOL_TIMER_SF8)
    {
    }
    class RachConfigCommon
    {
    public:RachConfigCommon ();
      PreambleInfo preambleInfo_;
      PowerRampingParameters powerRampingParameters_;
      RaSupervisionInfo raSupervisionInfo_;
      uint16_t maxHarqMsg3Tx_;
      bool isPartitionPLThresholdPresent_;
      uint16_t partitionPLThreshold_;
    };
    inline RachConfigCommon::
      RachConfigCommon ():maxHarqMsg3Tx_ (INIT_VALUE_16BITS),
      partitionPLThreshold_ (INIT_VALUE_16BITS)
    {
    };
    class RlcConfiguration
    {
    public:RlcConfiguration ();
    };
    class LogicalChannelConfig
    {
    public:LogicalChannelConfig ();
    };
    class SrbToAddModify
    {
    public:enum RlcConfigChoice
      {
	RLC_CONFIG_NOT_SPECIFIED = 0, RLC_CONFIG_EXPLICIT_CHOSEN =
	  1, RLC_CONFIG_DEFAULT_CHOSEN = 2
      };
      enum LogicalChannelConfigChoice
      {
	LOGICAL_CHANNEL_CONFIG_NOT_SPECIFIED =
	  1, LOGICAL_CHANNEL_CONFIG_DEFAULT_CHOSEN = 2
      };
        SrbToAddModify ();
      bool isRlcConfigurationPresent_;
      RlcConfigChoice rlcConfigChoice_;
      RlcConfiguration rlcConfiguration_;
      SrbIdentity srbIdenity_;
      bool isLogicalChannelConfigPresent_;
      LogicalChannelConfigChoice logicalChannelConfigChoice_;
    };
    inline SrbToAddModify::
      SrbToAddModify ():isRlcConfigurationPresent_ (false),
      rlcConfigChoice_ (RLC_CONFIG_NOT_SPECIFIED), srbIdenity_ (0),
      isLogicalChannelConfigPresent_ (false),
      logicalChannelConfigChoice_ (LOGICAL_CHANNEL_CONFIG_NOT_SPECIFIED)
    {
    }
    class RlcUm
    {
    public:RlcUm ();
    };
    class Profile
    {
    public:Profile ();
      bool profile0x0001_;
      bool profile0x0002_;
      bool profile0x0003_;
      bool profile0x0004_;
      bool profile0x0006_;
      bool profile0x0101_;
      bool profile0x0102_;
      bool profile0x0103_;
      bool profile0x0104_;
    };
    inline Profile::Profile ():profile0x0001_ (false), profile0x0002_ (false),
      profile0x0003_ (false), profile0x0004_ (false), profile0x0006_ (false),
      profile0x0101_ (false), profile0x0102_ (false), profile0x0103_ (false),
      profile0x0104_ (false)
    {
    }
    class HeaderCompression
    {
    public:HeaderCompression ();
      static const uint16_t DEFAULT_MAXCID = 15;
      uint16_t maxCid_;
      Profile profile_;
    };
    inline HeaderCompression::HeaderCompression ():maxCid_ (DEFAULT_MAXCID)
    {
    }
    enum DiscardTimer
    {
      DISCARD_TIMER_MS50 = 0, DISCARD_TIMER_MS100 = 1, DISCARD_TIMER_MS150 =
	6, DISCARD_TIMER_INF = 7, NUM_OF_DISCARD_TIMER, MIN_DISCARD_TIMER =
	DISCARD_TIMER_MS50, MAX_DISCARD_TIMER = NUM_OF_DISCARD_TIMER - 1
    };
    class PdcpConfiguration
    {
    public:PdcpConfiguration ();
      bool isDiscardTimerPresent_;
      DiscardTimer discardTimer_;
      bool isRlcAmPresent_;
      bool isRlcUmPresent_;
      RlcUm rlcUm_;
      bool isHeaderCompressionPresent_;
      HeaderCompression headerCompression_;
    };
    inline PdcpConfiguration::
      PdcpConfiguration ():isDiscardTimerPresent_ (false),
      discardTimer_ (NUM_OF_DISCARD_TIMER), isRlcAmPresent_ (false),
      isRlcUmPresent_ (false), isHeaderCompressionPresent_ (false)
    {
    }
    class DrbToAddModify
    {
    public:DrbToAddModify ();
      EpsBearerIdentity epsBearerIdentity_;
      DrbIdentity drbIdentity_;
      bool isPdcpConfigurationPresent_;
      PdcpConfiguration pdcpConfiguration_;
      bool isRlcConfigurationPresent_;
      RlcConfiguration rlcConfiguration_;
      bool isRBMappingInfoPresent_;
      DtchLogicalChannelId rBMappingInfo_;
      bool isLogicalChannelConfigPresent_;
      LogicalChannelConfig logicalChannelConfig_;
    };
    inline DrbToAddModify::
      DrbToAddModify ():epsBearerIdentity_ (MIN_EPS_BEARER_ID),
      drbIdentity_ (MIN_DRB_ID), isPdcpConfigurationPresent_ (false),
      isRlcConfigurationPresent_ (false), isRBMappingInfoPresent_ (false),
      rBMappingInfo_ (MIN_DTCH_LOGICAL_CHN_ID),
      isLogicalChannelConfigPresent_ (false)
    {
    }
    class RadioResourceConfigDedicated
    {
    public:enum ChannelConfigChoice
      {
      };
        RadioResourceConfigDedicated ();
      static const uint16_t MAX_SRB = 2;
      static const uint16_t MIN_SRB = 1;
      bool isSrbToAddModifyListPresent_;
        CommonList < SrbToAddModify, MIN_SRB, MAX_SRB, true > srbToAddModify_;
        CommonList < DrbToAddModify, MIN_DRB, MAX_DRB, true > drbToAddModify_;
      bool isSpsConfigurationPresent_;
    };
    inline RadioResourceConfigDedicated::
      RadioResourceConfigDedicated ():isSrbToAddModifyListPresent_ (false),
      isSpsConfigurationPresent_ (false)
    {
    }
    class ReportConfigEutra
    {
    };
    class EventB1
    {
    };
    class EventB2
    {
    };
    class EventRatId
    {
    public:typedef UnionContainer < EventB1, EventB2 > EventUnion;
    private:  EventUnion id_;
    };
    class EventRat
    {
      EventRatId eventId_;
    };
    class TriggerRatType
    {
      EventRat event_;
    };
    class ReportConfigInterRat
    {
      TriggerRatType triggerType_;
    };
    class ReportConfig
    {
    public:typedef UnionContainer < ReportConfigEutra,
	ReportConfigInterRat > ReportUnion;
    private:  ReportUnion data_;
    };
    class ReportConfigToAddModify
    {
      ReportConfig reportConfig_;
    };
    class MeasurementConfiguration
    {
      static const uint16_t MIN_NUM_REPORT_CONFIG_TOADD_MODIFY = 1;
      static const uint16_t MAX_NUM_REPORT_CONFIG_TOADD_MODIFY =
	MAX_MEAS_OBJECT_ID;
        CommonList < ReportConfigToAddModify,
	MIN_NUM_REPORT_CONFIG_TOADD_MODIFY,
	MAX_NUM_REPORT_CONFIG_TOADD_MODIFY,
	false > reportConfigToAddModifyList_;
    };
    class RadioResourceConfigCommon
    {
    public:RadioResourceConfigCommon ();
      RachConfigCommon rachConfigCommon_;
    };
    class MobilityControlInformation
    {
    public:MobilityControlInformation ();
      RadioResourceConfigCommon radioResourceConfigCommon_;
    };
    class RrcConnectionReconfiguration
    {
    public:RrcConnectionReconfiguration ();
      MeasurementConfiguration measurementConfiguration_;
      MobilityControlInformation mobilityControlInformation_;
        CommonList < NasDedicatedInformation, MIN_DRB, MAX_DRB,
	false > nasDedicatedInfoList_;
      RadioResourceConfigDedicated radioResourceConfigDedicated_;
    };
    inline RrcConnectionReconfiguration::RrcConnectionReconfiguration ()
    {
    }
  }
}

class HoUnitTest
{
  void buildX2HoReqAck (uint8_t * &buffer, uint32_t & length);
};
class RrcR2AsnCodecApi
{
public:static bool encodeRrcConnectionReconfiguration (const LteApp::R2RRC::
						  RrcConnectionReconfiguration
						  & applMsg,
						  uint8_t * &buffer,
						  uint32_t & length);
};
void
HoUnitTest::buildX2HoReqAck (uint8_t * &buffer, uint32_t & length)
{
  LteApp::R2RRC::RrcConnectionReconfiguration rrcConnReconfig;
  LteApp::R2RRC::RachConfigCommon rachConfigCommon;
  rachConfigCommon.preambleInfo_.numOfRaPreambles_ =
    LteApp::R2RRC::RA_PREAMBLES_N64;
  rrcConnReconfig.mobilityControlInformation_.radioResourceConfigCommon_.
    rachConfigCommon_ = rachConfigCommon;
  uint8_t *encodeBufPtr = 0;
  uint32_t bufLength = 0;
  RrcR2AsnCodecApi::encodeRrcConnectionReconfiguration (rrcConnReconfig,
							encodeBufPtr,
							bufLength);
}
