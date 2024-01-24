// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

AMainGameModeBase::AMainGameModeBase()
{
	SaveSystemComponent = CreateDefaultSubobject<USaveSystemComponentBase>(TEXT("SaveSystemComponentBase"));
}