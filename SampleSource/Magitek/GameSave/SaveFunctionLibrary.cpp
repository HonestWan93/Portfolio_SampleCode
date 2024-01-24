// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSave/SaveFunctionLibrary.h"

#pragma region Post
TArray<FPostInfoV1> USaveFunctionLibrary::ConvertPostInfoListToSaveStructArray(const TArray<FPostInfo>& PostInfoList)
{
	TArray<FPostInfoV1> postInfoListV1;
	for (auto postInfo : PostInfoList)
	{
		postInfoListV1.Add(ConvertPostInfoToSaveStruct(postInfo));
	}

	return postInfoListV1;
}

TArray<FPostInfo> USaveFunctionLibrary::ConvertSavePostInfoListToRuntimeStructArray(const TArray<FPostInfoV1>& PostInfoListV1)
{
	TArray<FPostInfo> postInfoList;
	for (auto postInfoV1 : PostInfoListV1)
	{
		postInfoList.Add(ConvertSavePostInfoToRuntimeStruct(postInfoV1));
	}

	return postInfoList;
}

FPostInfoV1 USaveFunctionLibrary::ConvertPostInfoToSaveStruct(const FPostInfo& PostInfo)
{
	FPostInfoV1 postInfoV1;
	postInfoV1.Description = PostInfo.Description;
	postInfoV1.EndTimestamp = PostInfo.EndTimestamp;
	postInfoV1.HasItem = PostInfo.HasItem;
	postInfoV1.IsReceive = PostInfo.IsReceive;
	postInfoV1.PostID = PostInfo.PostID;
	postInfoV1.Title = PostInfo.Title;
	postInfoV1.PostItemList = ConvertPostItemArrayToSaveStructArray(PostInfo.PostItemList);

	return postInfoV1;
}

FPostInfo USaveFunctionLibrary::ConvertSavePostInfoToRuntimeStruct(const FPostInfoV1& PostInfoV1)
{
	FPostInfo postInfo;
	postInfo.Description = PostInfoV1.Description;
	postInfo.EndTimestamp = PostInfoV1.EndTimestamp;
	postInfo.HasItem = PostInfoV1.HasItem;
	postInfo.IsReceive = PostInfoV1.IsReceive;
	postInfo.PostID = PostInfoV1.PostID;
	postInfo.Title = PostInfoV1.Title;
	postInfo.PostItemList = ConvertSavePostItemArrayToRuntimeStructArray(PostInfoV1.PostItemList);
	
	return postInfo;
}

TArray<FPostItemV1> USaveFunctionLibrary::ConvertPostItemArrayToSaveStructArray(TArray<FPostItem> PostItemArray)
{
	TArray<FPostItemV1> postItemV1Array;
	for (auto postItem : PostItemArray)
	{
		postItemV1Array.Add(ConvertPostItemToSaveStruct(postItem));
	}

	return postItemV1Array;
}

TArray<FPostItem> USaveFunctionLibrary::ConvertSavePostItemArrayToRuntimeStructArray(TArray<FPostItemV1> PostItemV1Array)
{
	TArray<FPostItem> postItemArray;
	for (auto postItemV1 : PostItemV1Array)
	{
		postItemArray.Add(ConvertSavePostItemToRuntimeStruct(postItemV1));
	}

	return postItemArray;
}

FPostItemV1 USaveFunctionLibrary::ConvertPostItemToSaveStruct(const FPostItem& postItem)
{
	FPostItemV1 postItemV1;
	postItemV1.ItemCount = PostItem.ItemCount;
	postItemV1.ItemID = PostItem.ItemID;

	return postItemV1;
}

FPostItem USaveFunctionLibrary::ConvertSavePostItemToRuntimeStruct(const FPostItemV1& PostItemV1)
{
	FPostItem postItem;
	postItem.ItemCount = PostItemV1.ItemCount;
	postItem.ItemID = PostItemV1.ItemID;

	return postItem;
}

#pragma endregion 