#include "DMLNotifier.h"
#include "..\DBModule.h"

using namespace NSDBModule;

void CDMLMonitor::Ring(DMLEvent* e)
{
	//ASSERT(e && e->TableID && (e->RecordFresh || e->RecordOrigin));
	if(e && (e->Command & Command2Monit) && (Table2Monit < 0 || Table2Monit == e->TableID))
	{
		Delegate(e);
	}
}

void CDMLNotifier::Dispatch(DMLEvent* e)
{
	for (MonitorMap::iterator iter = Monitors.begin(); iter != Monitors.end() && !e->FlagCancel; ++iter)
	{
		iter->second.Ring(e);
	}
}

int CDMLNotifier::AppendMonitor(flag_t cmd, int tbl, DelegateDMLMonit cb)
{
	if(MonitorSN + 1 < 0 || !cmd || !cb)
	{
		return -1;
	}
	++MonitorSN;
	Monitors.insert(std::make_pair(MonitorSN,CDMLMonitor(tbl, cmd, cb)));
	return MonitorSN;
}

void CDMLNotifier::RemoveMonitor(int id)
{
	MonitorMap::iterator iter = Monitors.find(id);
	if(iter == Monitors.end())
	{
		throw std::exception("ָ��ID�ļ����������ڣ�");
		//ASSERT(false);
	}
	else
	{
		Monitors.erase(iter);
	}
}