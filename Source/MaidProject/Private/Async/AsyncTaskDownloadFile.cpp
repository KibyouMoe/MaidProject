// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/AsyncTaskDownloadFile.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UAsyncTaskDownloadFile::UAsyncTaskDownloadFile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if ( HasAnyFlags(RF_ClassDefaultObject) == false )
	{
		AddToRoot();
	}
}


UAsyncTaskDownloadFile* UAsyncTaskDownloadFile::DownloadFile(FString URL, FString DownloadDir)
{
	UAsyncTaskDownloadFile* DownloadTask = NewObject<UAsyncTaskDownloadFile>();
	FString LeftStr;
	FString RightStr;
	URL.Split("/", &LeftStr, &RightStr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	const FString DownloadFilePath = FPaths::Combine(DownloadDir,RightStr);
	DownloadTask->SetFilePath(DownloadFilePath);
	DownloadTask->Start(URL);

	return DownloadTask;
}


void UAsyncTaskDownloadFile::SetFilePath(const FString& InFilePath)
{
	FilePath = InFilePath;
}


void UAsyncTaskDownloadFile::Start(const FString& URL)
{
#if !UE_SERVER
	// Create the Http request and add to pending request list
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAsyncTaskDownloadFile::HandleRequest);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->ProcessRequest();
#else
	// On the server we don't execute fail or success we just don't fire the request.
	RemoveFromRoot();
#endif
}

void UAsyncTaskDownloadFile::HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded)
{
#if !UE_SERVER

	RemoveFromRoot();

	if ( bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0 && HttpResponse->GetContent().Num() > 0 )
	{
		FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *FilePath);
		OnSuccess.Broadcast(FilePath);
	}

	OnFail.Broadcast("");

#endif
}
