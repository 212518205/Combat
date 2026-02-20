#pragma once

namespace Debug
{
	static void Print(const FString& Message, const float Time = 7.f, const FColor& Color = FColor::MakeRandomColor(),const int32 InKey = -1                  
	)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, Time, Color, Message);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}

	


	static void Print(const FString& Message, const float PrintFloat, const int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			const FString FinalString = Message + TEXT(": ") + FString::SanitizeFloat(PrintFloat);
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, InColor, FinalString);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalString);
		}
	}
}
