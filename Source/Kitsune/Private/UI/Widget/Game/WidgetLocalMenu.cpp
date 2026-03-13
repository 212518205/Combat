// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Game/WidgetLocalMenu.h"

#include "UIManagerSubsystem.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "UI/DataObjects/Inventory/TileViewEntryData.h"
#include "UI/Widget/Components/KitsuneCommonTileView.h"

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

        UTileViewEntryData* EntryData = NewObject<UTileViewEntryData>(this);
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

    TileView_MenuTile->ClearListItems();
    for (UTileViewEntryData* EntryData : CachedMenuTileData)
    {   
        if (EntryData->RequestLevel <= GetLocalPlayerViewModel()->GetLocalPlayerLevel()) {
            TileView_MenuTile->AddItem(EntryData);
        }
    }
}
