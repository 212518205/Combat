#include "UI/Widget/Game/WidgetLocalMenu.h"

#include "Components/WrapBox.h"
#include "CommonTypes/StructTypes.h"
#include "UI/ViewModel/ASCViewModel.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"
#include "UI/Widget/Components/Entry/MenuEntryMapping.h"
#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

void UWidgetLocalMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    RefreshMenu();
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
        EntryData->ShowAsNewText = TileRow->bShowAsNew ? FText::FromString(TEXT("新")) : FText::GetEmpty();
        EntryData->RequestLevel = TileRow->RequiredLevel;
        EntryData->EntryType = TileRow->EntryType;
        EntryData->ScreenToOpen = TileRow->ScreenToOpen;
        EntryData->NameID = RowNames[i];

        CachedMenuTileData.Add(EntryData);
    }

}

void UWidgetLocalMenu::RefreshMenu()
{
    LoadAllMenuTileData();
    WrapBox_Menu->ClearChildren();

    const UASCViewModel* ASCVM = GetViewModel<UASCViewModel>();
    if (!ASCVM)
    {
        UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] WidgetLocalMenu: ASCViewModel 未就绪，等级按 0 处理"));
    }
    const int32 PlayerLevel = ASCVM ? ASCVM->GetLocalPlayerLevel() : 0;

    for (UMenuEntryData* EntryData : CachedMenuTileData)
    {
        if (EntryData->RequestLevel <= PlayerLevel)
        {
            TSubclassOf<UMenuEntryBase> FoundEntryClass = TileEntryMapping->FindEntryClassByEntryType(EntryData->EntryType);

            UMenuEntryBase* WidgetEntry = CreateWidget<UMenuEntryBase>(this, FoundEntryClass);
            WidgetEntry->NativeOnEntryInitialize(EntryData);

            WrapBox_Menu->AddChildToWrapBox(WidgetEntry);
        }
    }
}
