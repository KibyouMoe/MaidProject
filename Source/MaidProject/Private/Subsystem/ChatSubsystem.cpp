// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ChatSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

DEFINE_LOG_CATEGORY_STATIC(LogChatSubsystem, Log, All)

void UChatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogChatSubsystem, Log, TEXT("聊天子系统初始化完成！（随着GameInstance一起）"))
	// 初始化参数
	bInited = false;
	// 注册 Url 委托
	
	// 尝试和Python服务器建立链接
	
	// 并发送初始化信息
	SendInitializedMsg();
}


void UChatSubsystem::SendInitializedMsg()
{
	const TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UChatSubsystem::OnResInitializedMsg);
	HttpRequest->SetURL("http://127.0.0.1:8000/init");
	HttpRequest->SetVerb(TEXT("POST"));	
	HttpRequest->ProcessRequest();
}

void UChatSubsystem::OnResInitializedMsg(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	const bool bSucceeded)
{
	if ( bSucceeded && HttpResponse.IsValid() )
	{
		if (HttpResponse->GetContentLength() > 0 && HttpResponse->GetContent().Num() > 0)
		{
			const FString RespondStr = HttpResponse->GetContentAsString();
			UE_LOG(LogChatSubsystem, Log, TEXT("请求成功：%s"), *RespondStr)
			bInited = true;
		}
		else
		{
			const int32 RespondCode = HttpResponse->GetResponseCode();
			UE_LOG(LogChatSubsystem, Log, TEXT("请求失败：%d"), RespondCode)
		}
	}
	else
	{
		UE_LOG(LogChatSubsystem, Log, TEXT("请求失败"))
	}
}

void UChatSubsystem::Deinitialize()
{
	
	UE_LOG(LogChatSubsystem, Log, TEXT("聊天子系统析构完成！（随着GameInstance一起）"))
	Super::Deinitialize();
}

// USoundWave* UChatSubsystem::GetSoundWaveFromFile(const FString& FilePath)
// {
// 	TArray<uint8> RawData;
// 	FFileHelper::LoadFileToArray(RawData, *FilePath);
// 	if (RawData.Num() > 0)
// 	{
// 		return GetSoundWaveFromArray(RawData);
// 	}
// 	return nullptr;
// }

// USoundWave* UChatSubsystem::GetSoundWaveFromArray(const TArray<uint8>& RawData)
// {
// 	FWaveModInfo WaveInfo;
// 	if (!WaveInfo.ReadWaveInfo(RawData.GetData(), RawData.Num()))
// 	{
// 		return nullptr;
// 	}
// 		
// 	USoundWave* SoundWave = NewObject<USoundWave>();
// 	
// 	const int32 ChannelCount = *WaveInfo.pChannels;
// 	check(ChannelCount > 0);
//  
// 	const int32 SizeOfSample = (*WaveInfo.pBitsPerSample) / 8;
//  
// 	const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
// 	const int32 NumFrames = NumSamples / ChannelCount;
//
// 		
// 	const FSharedBuffer UpdatedBuffer = FSharedBuffer::Clone(RawData.GetData(), RawData.Num() * sizeof(uint8));
// 	SoundWave->RawData.UpdatePayload(UpdatedBuffer);
//  
// 	SoundWave->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
// 	SoundWave->SetImportedSampleRate(*WaveInfo.pSamplesPerSec);
// 	SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
// 	SoundWave->NumChannels = ChannelCount;
// 	SoundWave->TotalSamples = *WaveInfo.pSamplesPerSec * SoundWave->Duration;
// 	
//
// 	return SoundWave;
// }
