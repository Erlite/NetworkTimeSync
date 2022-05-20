/*
MIT License

Copyright (c) 2022 Younes Meziane

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NetworkTimeSyncSettings.generated.h"

/**
 * Contains configuration for the UNetworkTimeSubsystem.
 */
UCLASS(Config = "Game", DefaultConfig, Meta = (DisplayName = "Network Time Sync Settings"))
class NETWORKTIMESYNC_API UNetworkTimeSyncSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/** Whether or not to use unreliable RPCs to synchronize time. Enable if you set the interval to update frequently. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bUseUnreliableRPCs = false;

	/**
	 * Time between each synchronization attempt in seconds.
	 * For very frequent updates, enable bUseUnreliableRPCs to avoid saturating the bandwidth and killing your game.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = "1.0", ValueMin = "1.0"))
	float NetworkSyncIntervalSeconds = 10.0f;
};
