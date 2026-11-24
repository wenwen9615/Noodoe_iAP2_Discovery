# NewDoe iAP2 Discovery v09 — 裝置探索實驗（原始碼）

不是安裝 ZIP、不是完整 iAP2，也不能直接刷入儀表。目標：在作者的 Product C 原始碼中驗證 iPhone 配件選擇器是否可發現 NewDoe，並準備獨立 RFCOMM/SDP 入口供真正的 iAP2 backend 使用。

## 來源
- 1.0.1.0 stock.bin SHA256：162faeecc64bf56d828570168785e66d1a7da0152ef912bf83f9a19094d91edf。
- cfw.bin SHA256：2523b2e94a0df528486751ebf8acb57431f39fc8ff1ce46b13b992a6dd625d7a。
- 原廠 EIR 解壓縮結果見 stock-evidence.txt。EIR UUID 採反向位元組排列，SDP 使用 canonical byte order。
- GitHub ZIP 的 Projects/documentations/architecture.md 說明 Product 的 BT/NDCP 與 Gate 分離。公開 ZIP 不含 C/H 原始碼，因此這不是已整合的 patch。

## 實機整合（需要作者）
1. 將 src/ 加入 Product CFW，確認 pinned BTstack API 與記憶體池。取得現有完整 240-byte EIR（包含實際名稱與所有服務），不可用空白 EIR 覆蓋既有內容。若由 BTstack 自動產生，可從原始碼或實際 HCI 0x0C52 取得。
2. 在既有 RFCOMM/SDP 初始化及 SPP 註冊後、radio power-on 前，僅在台架測試 build 設定 `-DND_IAP_ENABLE_DISCOVERY_ONLY=1`，呼叫 `nd_iap_lab_eir_only(existing_eir)` 並檢查回傳值。維持原有 SSP 安全設定、配對金鑰及 RecoveryGate。不修改安裝與恢復流程。
3. 使用原本配對模式，確認 Android SPP 仍正常，再從 NoodoeProbe 的 ExternalAccessory 配件選擇器搜尋。觀察 HCI 0x0C52 Command Complete 狀態、Android SDP 結果及 iPhone 搜尋畫面。此模式只有 EIR，沒有 iAP SDP、RFCOMM、MFi 認證，看到裝置也不代表能配對。測完恢復正常 build。勿在實車或行駛時進行首次實驗。
4. 有真正相容且經授權的 iAP2 backend 後，改呼叫 `nd_iap_register(existing_eir, existing_spp_channel, unused_iap_channel, backend_packet_handler)`。建立候選 SDP 記錄及獨立 RFCOMM 服務，成功後才提交 EIR。仍需確認實際 SDP 屬性、遠端 SDP 查詢、連線方向、記憶體池與安全策略。兩個模式不得同時呼叫；目前只支援一次啟動註冊，radio restart、動態停用及清理須原作者整合。
5. 真正配對需完整 iAP2 Link、Control、有效的配件識別、實體 MFi 晶片認證及 EA Session；不能用假認證回覆或複製他人憑證。保持實驗通道只讀，不接韌體更新 API。不要公開配對金鑰或認證材料。

## 驗證範圍
- test-results.txt 是本機執行結果。EIR core 通過 GCC ASan/UBSan 並編譯成 Cortex-M4 Thumb 物件；438 bytes 是單一 core object 的 text，非完整 iAP2 大小。
- BTstack adapter 使用本機 mock headers/functions 測試呼叫順序、失敗回復及 lab mode，未使用真實 BTstack 或 STM32 工程，也未實測 RF、iPhone 配對、認證。
- `make test` 可重現 host tests，但終端機測試並非使用者要的實機配對。本 ZIP 不含可刷寫映像。

## Message for the developer
Could you integrate this small EIR-only experiment into a bench Product build, keeping the existing SPP and security configuration? We need the existing EIR builder, pinned BTstack revision/config and current RFCOMM/SDP lifecycle. Please verify HCI 0x0C52 command-complete status. I can test the iPhone ExternalAccessory picker. A separate RFCOMM endpoint can be enabled later with a real authorized iAP2 backend. No fake authentication or production deployment of an unsupported service.

## Public API references
- https://bluekitchen-gmbh.com/btstack/develop/appendix/gap/
- https://bluekitchen-gmbh.com/btstack/develop/appendix/spp_server/
- https://bluekitchen-gmbh.com/btstack/develop/appendix/rfcomm/
- https://bluekitchen-gmbh.com/btstack/develop/appendix/sdp_server/
- https://developer.apple.com/documentation/externalaccessory/eaaccessorymanager/showbluetoothaccessorypicker(withnamefilter:completion:)
