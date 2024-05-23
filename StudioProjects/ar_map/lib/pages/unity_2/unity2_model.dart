import '/flutter_flow/flutter_flow_util.dart';
import 'unity2_widget.dart' show Unity2Widget;
import 'package:flutter/material.dart';

class Unity2Model extends FlutterFlowModel<Unity2Widget> {
  ///  State fields for stateful widgets in this page.

  final unfocusNode = FocusNode();

  @override
  void initState(BuildContext context) {}

  @override
  void dispose() {
    unfocusNode.dispose();
  }
}
