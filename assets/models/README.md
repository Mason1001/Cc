# YOLOv8 Model Placeholder

This directory is intended for YOLOv8 ONNX model files.

## Required Model

Place your YOLOv8 ONNX model file here:
- `yolov8n.onnx` (nano - fastest, least accurate)
- `yolov8s.onnx` (small)
- `yolov8m.onnx` (medium)
- `yolov8l.onnx` (large)
- `yolov8x.onnx` (extra large - slowest, most accurate)

## Downloading Models

### Option 1: From Ultralytics

Visit: https://github.com/ultralytics/ultralytics

Download pre-trained ONNX models directly from their releases.

### Option 2: Export from PyTorch

If you have Python and Ultralytics installed:

```bash
pip install ultralytics

# Export to ONNX
from ultralytics import YOLO
model = YOLO('yolov8n.pt')
model.export(format='onnx')
```

### Option 3: Direct Download

Some ONNX models are available from various sources. Ensure they are:
- YOLOv8 architecture
- ONNX format (.onnx extension)
- Input shape: [1, 3, 640, 640] (default)

## Model Information

### YOLOv8n (Recommended for Gaming)
- Size: ~6 MB
- Speed: ~1-2ms on GPU
- Accuracy: Good for real-time use
- Best for: Gaming overlays, real-time detection

### YOLOv8s
- Size: ~22 MB
- Speed: ~2-4ms on GPU
- Accuracy: Better than nano
- Best for: Balanced performance

### YOLOv8m
- Size: ~52 MB
- Speed: ~5-10ms on GPU
- Accuracy: High
- Best for: Quality over speed

## Configuration

The application looks for models at:
```
assets/models/yolov8n.onnx
```

To use a different model, update the path in `src/main.cpp`:
```cpp
g_objectDetector->Initialize("assets/models/your_model.onnx");
```

## Notes

- Models are not included in the repository due to size
- Demo mode works without a model (shows test detections)
- Real detection requires ONNX Runtime + model file
- GPU acceleration significantly improves performance

## Testing Without Model

The application will run in demo mode without a model:
- Detection checkbox can be enabled
- Shows animated test bounding boxes
- No actual object detection occurs
- Useful for testing UI and rendering

## Model Performance

Expected performance on modern gaming GPU (RTX 3060):
- YOLOv8n: 1-2ms (~500-1000 FPS)
- YOLOv8s: 2-4ms (~250-500 FPS)
- YOLOv8m: 5-10ms (~100-200 FPS)
- YOLOv8l: 10-20ms (~50-100 FPS)
- YOLOv8x: 20-40ms (~25-50 FPS)

For 120 FPS overlay, use YOLOv8n or YOLOv8s.

## Classes

Default COCO dataset includes 80 classes:
- person, bicycle, car, motorcycle, airplane, bus, train, truck, boat
- traffic light, fire hydrant, stop sign, parking meter, bench
- bird, cat, dog, horse, sheep, cow, elephant, bear, zebra, giraffe
- backpack, umbrella, handbag, tie, suitcase, frisbee, skis, snowboard
- sports ball, kite, baseball bat, baseball glove, skateboard, surfboard
- tennis racket, bottle, wine glass, cup, fork, knife, spoon, bowl
- banana, apple, sandwich, orange, broccoli, carrot, hot dog, pizza
- donut, cake, chair, couch, potted plant, bed, dining table, toilet
- tv, laptop, mouse, remote, keyboard, cell phone, microwave, oven
- toaster, sink, refrigerator, book, clock, vase, scissors, teddy bear
- hair drier, toothbrush

For gaming, you'll typically want to detect the 'person' class.

## License

YOLOv8 models are released under AGPL-3.0 license by Ultralytics.
Ensure compliance when using in your projects.

## Support

For model-related issues:
- Check ONNX Runtime is installed
- Verify model format is correct
- Ensure input dimensions match
- Test with different models if one doesn't work
