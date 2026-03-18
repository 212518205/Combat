// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"

void UMenuEntryBase::NativeOnEntryInitialize(UObject* ListItemObject)
{
	if (UMenuEntryData* EntryData = Cast<UMenuEntryData>(ListItemObject))
	{
		CachedEntryData = EntryData;

		Image_DisplayImage->SetBrushFromTexture(EntryData->DisplayIcon);
		CommonTextBlock_DisplayName->SetText(EntryData->DisplayName);
		CommonTextBlock_HotKeyText->SetText(EntryData->HotKeyText);
		CommonTextBlock_ShowAsNewText->SetText(EntryData->ShowAsNewText);
	}

}

void UMenuEntryBase::MouseEnter()
{
	Image_DisplayImage->SetColorAndOpacity(HoverIconColor);

	Super::MouseEnter();
}

void UMenuEntryBase::MouseLeave()
{
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);

	Super::MouseLeave();
}

void UMenuEntryBase::MouseDown()
{
	Image_DisplayImage->SetColorAndOpacity(ClickIconColor);

	Super::MouseDown();
}

void UMenuEntryBase::MouseUp()
{
	Image_DisplayImage->SetColorAndOpacity(DefaultIconColor);

	Super::MouseUp();
}
