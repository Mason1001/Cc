# YOLOv8 Models Directory

Place your YOLOv8 model files in this directory.

## Supported Models

### PyTorch Models (.pt)
- yolov8n.pt (Nano - fastest)
- yolov8s.pt (Small)
- yolov8m.pt (Medium)
- yolov8l.pt (Large)
- yolov8x.pt (Extra Large - most accurate)

### ONNX Models (.onnx)
- yolov8n.onnx
- yolov8s.onnx
- yolov8m.onnx
- yolov8l.onnx
- yolov8x.onnx

## Download Instructions

1. Visit Ultralytics YOLOv8: https://github.com/ultralytics/ultralytics
2. Download pre-trained models:

```bash
# Using Python and ultralytics package
pip install ultralytics
python -c "from ultralytics import YOLO; model = YOLO('yolov8n.pt')"
```

3. Or download directly from releases:
   - https://github.com/ultralytics/assets/releases

4. Place the downloaded model file in this directory

## Configuration

Update the model path in `settings.json`:

```json
"detection": {
  "model_path": "models/yolov8n.pt",
  ...
}
```

## Model Performance Comparison

| Model | Size (MB) | mAPval 50-95 | Speed (ms) |
|-------|-----------|--------------|------------|
| YOLOv8n | 6.2 | 37.3 | 80 |
| YOLOv8s | 21.5 | 44.9 | 128 |
| YOLOv8m | 49.7 | 50.2 | 234 |
| YOLOv8l | 83.7 | 52.9 | 375 |
| YOLOv8x | 130.5 | 53.9 | 479 |

Speed measured on NVIDIA V100 GPU.

## Custom Models

You can also use custom-trained YOLOv8 models:
1. Train your model using Ultralytics
2. Export to ONNX format for best compatibility
3. Place in this directory
4. Update settings.json with the model path and class names
