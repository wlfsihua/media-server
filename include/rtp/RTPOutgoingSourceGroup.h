#ifndef RTPOUTGOINGSOURCEGROUP_H
#define RTPOUTGOINGSOURCEGROUP_H

#include <string>
#include <map>
#include <set>

#include "config.h"
#include "use.h"
#include "rtp/RTPPacket.h"
#include "rtp/RTPOutgoingSource.h"


struct RTPOutgoingSourceGroup
{
public:
	class Listener 
	{
		public:
			virtual void onPLIRequest(RTPOutgoingSourceGroup* group,DWORD ssrc) = 0;
			virtual void onREMB(RTPOutgoingSourceGroup* group,DWORD ssrc,DWORD bitrate) = 0;
		
	};
public:
	RTPOutgoingSourceGroup(MediaFrame::Type type);
	RTPOutgoingSourceGroup(std::string &streamId,MediaFrame::Type type);
	
	void AddListener(Listener* listener);
	void RemoveListener(Listener* listener);
	void onPLIRequest(DWORD ssrc);
	void onREMB(DWORD ssrc,DWORD bitrate);
	
	RTPOutgoingSource* GetSource(DWORD ssrc);
	
	//RTX packets
	void AddPacket(const RTPPacket::shared& packet);
	RTPPacket::shared GetPacket(WORD seq) const;
	void ReleasePackets(QWORD until);
	
public:	
	std::string streamId;
	MediaFrame::Type type;
	RTPOutgoingSource media;
	RTPOutgoingSource fec;
	RTPOutgoingSource rtx;
private:	
	mutable Mutex mutex;
	std::map<DWORD,RTPPacket::shared> packets;
	std::set<Listener*> listeners;
};


#endif /* RTPOUTGOINGSOURCEGROUP_H */

