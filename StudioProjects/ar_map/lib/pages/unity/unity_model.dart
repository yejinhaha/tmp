import '/flutter_flow/flutter_flow_util.dart';
import 'unity_widget.dart' show UnityWidget;
import 'package:flutter/material.dart';

class UnityModel extends FlutterFlowModel<UnityWidget> {
  ///  State fields for stateful widgets in this page.

  final unfocusNode = FocusNode();

  @override
  void initState(BuildContext context) {}

  @override
  void dispose() {
    unfocusNode.dispose();
  }
}
