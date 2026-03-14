// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetLocalMenu.h"

#include "UIManagerSubsystem.h"
#include "Components/WrapBox.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"
#include "UI/Widget/Components/Entry/MenuEntryMapping.h"
#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

void UWidgetLocalMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    RefreshMenu();
}

UPlayerViewModel* UWidgetLocalMenu::GetLocalPlayerViewModel()
{
    if (!CachedLocalViewModel)
    {
        CachedLocalViewModel = UUIManagerSubsystem::GetUIManager(GetOwningPlayer())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPlayerPawn());
    }

    return CachedLocalViewModel;
}

void UWidgetLocalMenu::LoadAllMenuTileData()
{
	if (!CachedMenuTileData.IsEmpty())return;

    TArray<FMenuTileRow*> AllMenuDataRows;
    MenuDataTable->GetAllRows<FMenuTileRow>(TEXT("菜单信息"), AllMenuDataRows);

    TArray<FName> RowNames = MenuDataTable->GetRowNames();

    for (int32 i = 0; i < AllMenuDataRows.Num(); i++)
    {
        const FMenuTileRow* TileRow = AllMenuDataRows[i];

        UMenuEntryData* EntryData = NewObject<UMenuEntryData>(this);
        EntryData->DisplayIcon = TileRow->Icon.LoadSynchronous();
        EntryData->DisplayName = TileRow->DisplayName;
        EntryData->HotKeyText = TileRow->HotKeyText;
        EntryData->ShowAsNewText = TileRow->bIsShowAsNew ? FText::FromString(TEXT("新")) : FText::GetEmpty();
        EntryData->RequestLevel = TileRow->RequiredLevel;
        EntryData->EntryType = TileRow->EntryType;
        EntryData->NameID = RowNames[i];

        CachedMenuTileData.Add(EntryData);
    }

}

void UWidgetLocalMenu::RefreshMenu()
{
    LoadAllMenuTileData();
    WrapBox_Menu->ClearChildren();

    for (UMenuEntryData* EntryData : CachedMenuTileData)
    {
        if (EntryData->RequestLevel <= GetLocalPlayerViewModel()->GetLocalPlayerLevel())
        {
            TSubclassOf<UMenuEntryBase> FoundEntryClass = TileEntryMapping->FindEntryClassByEntryType(EntryData->EntryType);

            UMenuEntryBase* WidgetEntry = CreateWidget<UMenuEntryBase>(this, FoundEntryClass);
            WidgetEntry->NativeOnEntryInitialize(EntryData);

            WrapBox_Menu->AddChildToWrapBox(WidgetEntry);
        }
    }
}
