// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Trait/ItemTrait_Interact.h"


/*** TODO: 可改为根据输入的参数返回不同形式的提示文本... [2026年2月14日 17:44:34 来自`@BC@`] ***/
FText UItemTrait_Interact::GetPromptMessage() const
{
    const FText KeyName = FText::FromString(PromptKey.GetDisplayName().ToString());
    FText Format = FText::Format(NSLOCTEXT("Interaction", "PromptFormat", "按 [{0}] {1}"),
        KeyName,
        InteractPrompt);
    return Format;
}
