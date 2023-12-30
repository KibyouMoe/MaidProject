// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "AsyncTaskDownloadFile.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDownloadFileDelegate, FString, RetFilePath);

UCLASS()
class MAIDPROJECT_API UAsyncTaskDownloadFile : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true" ))
	static UAsyncTaskDownloadFile* DownloadFile(FString URL, FString DownloadDir);
	
	UPROPERTY(BlueprintAssignable)
	FDownloadFileDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FDownloadFileDelegate OnFail;

	void SetFilePath(const FString& InFilePath);
	
	void Start(const FString& URL);

private:

	/** Handles image requests coming from the web */
	void HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString FilePath;
	
};
