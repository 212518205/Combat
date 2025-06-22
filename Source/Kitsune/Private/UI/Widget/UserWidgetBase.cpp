// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UserWidgetBase.h"

void UUserWidgetBase::SetCharacterStateViewModel(UCharacterStateViewModel* CS_VM)
{
	if (!CSViewModel)
	{
		CSViewModel = CS_VM;
	}
	CharacterStateViewModelSet();
}
