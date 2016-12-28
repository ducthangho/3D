#pragma once
#include "Server.h"
#include <grpc++/grpc++.h>
#include "ymax.grpc.pb.h"
#include "yservice.pb.h"
#include "yservice.grpc.pb.h"

using yproto::NumFaceRange;
using yproto::ObjList;
using grpc::Status;
using yproto::YPrepare;

using y3d::Tools;
using y3d::RenameParam;
using y3d::ResultReply;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

Status YServiceImpl::RenameObject(ServerContext* context, const RenameParam* request,
	ResultReply* reply) 
{

	auto ret = InvokeAsync([]() -> void {
		INodeTab nRef;
		GetSceneNodes(nRef, nullptr);
		auto* ip = GetCOREInterface();

		auto* nameMaker = ip->NewNameMaker();
		GetCOREInterface13()->SetNameSuffixLength(2);
		std::map<MSTR, int> mnames;

		for (int i = 0; i < nRef.Count(); ++i) {
			auto* node = nRef[i];
			MSTR nodeName(node->GetName());
			MSTR tmp(nodeName);
			tmp.toLower();
			if (mnames.find(tmp) == mnames.end()) {
				mnames[tmp]++;
			}
			else if (mnames[tmp]++ > 0) {
				mprintf(L"Renaming %s ", nodeName);
				nameMaker->MakeUniqueName(nodeName);
				node->SetName(nodeName.data());
				tmp = nodeName;
				tmp.toLower();
				mnames[tmp]++;
				mprintf(L"to %s \n", nodeName);
			};
		}
		mprintf(L"Hello world\n");
	});
	reply->set_message("DDDD");
	//reply->message.set_message("ddd");
	waitForReturn(ret);
	return Status::OK;
	// ... (pre-existing code)
	}