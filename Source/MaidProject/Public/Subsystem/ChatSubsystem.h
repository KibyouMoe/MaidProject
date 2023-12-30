// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "ChatSubsystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MAIDPROJECT_API UChatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	// UFUNCTION(BlueprintCallable)
	// static USoundWave* GetSoundWaveFromFile(const FString& FilePath);
	//
	//
	// static USoundWave* GetSoundWaveFromArray(const TArray<uint8>& RawData);

private:
	void SendInitializedMsg();

	void OnResInitializedMsg(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded);

	bool bInited;
	
};

struct FWAVFileHeader
{
	char ChunkID[4];		//"RIFF"
	uint32 ChunkSize;		//文件大小减去8
	char Format[4];			//"WAVE"

	char SubchunkiID[4];	//"fmt"
	uint32 Subchunk1Size;	//PCM为16
	uint16 AudioFormat;		//PCM为1
	uint16 NumChannels;		//通道数
	uint32 SampleRate;		//采样率
	uint32 ByteRate;		//字节率
	uint16 BlockAlign;		//数据块对齐单位
	uint16 BitsPerSample;	//每个采样点的位数

	char Subchunk2ID[4];	//"data"
	uint32 Subchunk2Size;	//数据大小"
};

struct FWAVHeader
{
	char ChunkID[4];		//"RIFF"
	uint32 ChunkSize;		//文件大小减去8
	char Format[4];			//"WAVE"
};

struct FWAVFmt
{
	char SubchunkiID[4];	//"fmt"
	uint32 Subchunk1Size;	//PCM为16
	uint16 AudioFormat;		//PCM为1
	uint16 NumChannels;		//通道数
	uint32 SampleRate;		//采样率
	uint32 ByteRate;		//字节率
	uint16 BlockAlign;		//数据块对齐单位
	uint16 BitsPerSample;	//每个采样点的位数
};

struct FWAVData
{
	char Subchunk2ID[4];	//"data"
	uint32 Subchunk2Size;	//数据大小"
};